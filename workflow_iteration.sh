#!/bin/bash
#
# *** 版本 6：參數化的工作流助手腳本 (更靈活) ***
#
# 用法:
# ./run_all_workflow.sh <SYSTEM_NAME> <PATTERNS> [OPTIONAL_SBATCH_OPTIONS]
#
# 例如:
# 1. 立即執行:
#    ./run_all_workflow.sh hebb 800
# 2. 帶有依賴:
#    ./run_all_workflow.sh hebb 1200 "--dependency=afterok:12345"
# 3. 延遲執行:
#    ./run_all_workflow.sh hebb 1400 "--begin=now+2hour"
#
# 它會提交一個 8 步驟的作業鏈，並打印出"最後一個步驟(Step 8)的 Job ID"

# ===================================================
#           <<< 1. 獲取傳入的參數 >>>
# ===================================================
if [ -z "$1" ] || [ -z "$2" ]; then
    echo "錯誤: 缺少參數!"
    echo "用法: $0 <SYSTEM_NAME> <PATTERNS> [OPTIONAL_SBATCH_OPTIONS]"
    exit 1
fi

SYSTEM_NAME=$1
PATTERNS=$2
# 第三個參數是完整的 sbatch 選項字串 (例如 "--begin=now+1hour")
SBATCH_OPTIONS=$3

# ===================================================
#           <<< 2. 根據參數設定變數 >>>
# ===================================================
SYSTEM_CONFIG="config/std_systems/${SYSTEM_NAME}.yml"
GSTATS_NAME="${SYSTEM_NAME}_conductances"

# --- 固定的設定檔 ---
TRAIN_RUN_CONFIG="config/runtypes/default_train.yml"
SPONT_RUN_CONFIG="config/runtypes/spontaneous.yml"
COND_RUN_CONFIG="config/runtypes/conductances.yml"
PERT_RUN_CONFIG="config/runtypes/perturbation.yml"

# --- 步驟特定參數 ---
ACT_RUN_NAME="spontaneous"
GSTATS_NAMESPACE="lognormal"

# ===================================================
#           <<< 3. 建立日誌和參數檔 >>>
# ===================================================
RUN_TIMESTAMP=$(date +%Y-%m-%d_%H%M%S)
LOG_DIR="slurm_logs/${SYSTEM_NAME}_${PATTERNS}_${RUN_TIMESTAMP}"
mkdir -p $LOG_DIR
PARAM_FILE="${LOG_DIR}/run_parameters.txt"
cat > $PARAM_FILE << EOF
# -------------------------------------------------
# Workflow Parameters for Run: ${RUN_TIMESTAMP}
# System: ${SYSTEM_NAME}
# Patterns: ${PATTERNS}
# SBATCH_OPTIONS: ${SBATCH_OPTIONS}
# -------------------------------------------------
PATTERNS=${PATTERNS}
SYSTEM_NAME="${SYSTEM_NAME}"
SYSTEM_CONFIG="${SYSTEM_CONFIG}"
GSTATS_NAME="${GSTATS_NAME}"
EOF

# ===================================================
#           <<< 4. 提交作業鏈 >>>
# ===================================================
JOB_NAME_SUFFIX="${SYSTEM_NAME}_${PATTERNS}"

# 步驟 1: genconn
# *** 關鍵修改：將 $SBATCH_OPTIONS 直接傳給 sbatch ***
# 注意：$SBATCH_OPTIONS 變數 *不* 需要引號，這樣 shell 才能正確解析它
# (例如，把它看作 --begin 和 now+1hour 兩個參數)
JOB1_ID=$(sbatch \
    $SBATCH_OPTIONS \
    --job-name=step1_genconn_${JOB_NAME_SUFFIX} \
    --output="${LOG_DIR}/step1_genconn_%j.out" \
    --error="${LOG_DIR}/step1_genconn_%j.err" \
    slurm_scripts/run_genconn.sh $PATTERNS | awk '{print $4}')

# 步驟 2-8: 保持不變 (它們的依賴關係在工作流 *內部*)
JOB2_ID=$(sbatch \
    --job-name=step2_train_${JOB_NAME_SUFFIX} \
    --dependency=afterok:$JOB1_ID \
    --output="${LOG_DIR}/step2_train_%j.out" \
    --error="${LOG_DIR}/step2_train_%j.err" \
    slurm_scripts/run_simulation.sh $SYSTEM_CONFIG $TRAIN_RUN_CONFIG $PATTERNS | awk '{print $4}')

JOB3_ID=$(sbatch \
    --job-name=step3_spontaneous_${JOB_NAME_SUFFIX} \
    --dependency=afterok:$JOB2_ID \
    --output="${LOG_DIR}/step3_spontaneous_%j.out" \
    --error="${LOG_DIR}/step3_spontaneous_%j.err" \
    slurm_scripts/run_simulation.sh $SYSTEM_CONFIG $SPONT_RUN_CONFIG $PATTERNS | awk '{print $4}')

JOB4_ID=$(sbatch \
    --job-name=step4_activations_${JOB_NAME_SUFFIX} \
    --dependency=afterok:$JOB3_ID \
    --output="${LOG_DIR}/step4_activations_%j.out" \
    --error="${LOG_DIR}/step4_activations_%j.err" \
    slurm_scripts/run_activations.sh $PATTERNS $SYSTEM_NAME $ACT_RUN_NAME | awk '{print $4}')

JOB5_ID=$(sbatch \
    --job-name=step5_conductances_${JOB_NAME_SUFFIX} \
    --dependency=afterok:$JOB4_ID \
    --output="${LOG_DIR}/step5_conductances_%j.out" \
    --error="${LOG_DIR}/step5_conductances_%j.err" \
    slurm_scripts/run_simulation.sh $SYSTEM_CONFIG $COND_RUN_CONFIG $PATTERNS | awk '{print $4}')

JOB6_ID=$(sbatch \
    --job-name=step6_gstats_${JOB_NAME_SUFFIX} \
    --dependency=afterok:$JOB5_ID \
    --output="${LOG_DIR}/step6_gstats_%j.out" \
    --error="${LOG_DIR}/step6_gstats_%j.err" \
    slurm_scripts/run_gstats.sh $GSTATS_NAME $GSTATS_NAMESPACE $PATTERNS | awk '{print $4}')

JOB7_ID=$(sbatch \
    --job-name=step7_perturbation_${JOB_NAME_SUFFIX} \
    --dependency=afterok:$JOB6_ID \
    --output="${LOG_DIR}/step7_perturbation_%j.out" \
    --error="${LOG_DIR}/step7_perturbation_%j.err" \
    slurm_scripts/run_simulation.sh $SYSTEM_CONFIG $PERT_RUN_CONFIG $PATTERNS | awk '{print $4}')

JOB8_ID=$(sbatch \
    --job-name=step8_linear_${JOB_NAME_SUFFIX} \
    --dependency=afterok:$JOB7_ID \
    --output="${LOG_DIR}/step8_linear_%j.out" \
    --error="${LOG_DIR}/step8_linear_%j.err" \
    slurm_scripts/run_linear.sh $SYSTEM_NAME $PATTERNS | awk '{print $4}')

# 打印最後一個 Job ID (雖然在這個腳本中我們不會用到它，但保留是好的)
echo $JOB8_ID