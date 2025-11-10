#!/bin/bash
#====================================================================
#          8-Step Workflow Pipeline Submission Script
#====================================================================

echo "===================================================="
echo "          Starting 8-Step Workflow Pipeline         "
echo "===================================================="

# ===================================================
#           <<< NEW: Step Control >>>
# ===================================================
START_STEP=${1:-1}
END_STEP=${2:-8}

echo "將執行工作流從 STEP ${START_STEP} 到 STEP ${END_STEP}."
if [ $START_STEP -gt 1 ]; then
    echo "警告: 你從 Step ${START_STEP} 開始。"
    echo "請務必確認 Step 1 到 $(($START_STEP - 1)) 的步驟都已成功完成。"
fi

# ===================================================
#           <<< CONFIGURATION PARAMETERS >>>
# ===================================================
# --- Main parameters ---
PATTERNS=1000
SYSTEM_NAME="long_mix_hebb" # 'hebb', 'rate', or 'hebb_smooth_rate'

# --- log ---
NETWORK_CONFIG="config/networks/segment_chain.yml"
SYSTEM_CONFIG="config/systems/${SYSTEM_NAME}.yml"
TRAIN_RUN_CONFIG="config/runtypes/default_train_long.yml" # for default_train_long.yml elongated to 6000 s
SPONT_RUN_CONFIG="config/runtypes/spontaneous.yml"
COND_RUN_CONFIG="config/runtypes/conductances.yml"
PERT_RUN_CONFIG="config/runtypes/perturbation.yml"

# --- Step-Specific Parameters ---
# Step 4 (Activations)
ACT_RUN_NAME="spontaneous" 
# Step 6 (GStats)
GSTATS_NAME="${SYSTEM_NAME}_conductances"
GSTATS_NAMESPACE="lognormal"

# ===================================================
#                 LOGGING SETUP
# ===================================================
RUN_TIMESTAMP=$(date +%Y-%m-%d_%H%M%S)
LOG_DIR="slurm_logs/run_${RUN_TIMESTAMP}_steps_${START_STEP}-${END_STEP}"
mkdir -p $LOG_DIR

# --- *** Save Parameters to Log File *** ---
PARAM_FILE="${LOG_DIR}/run_parameters.txt"
echo "Saving parameters to ${PARAM_FILE}..."

cat > $PARAM_FILE << EOF
# -------------------------------------------------
# Workflow Parameters for Run: ${RUN_TIMESTAMP}
# -------------------------------------------------

# --- Main Parameters ---
PATTERNS=${PATTERNS}
SYSTEM_NAME="${SYSTEM_NAME}"

# --- Config Files ---
SYSTEM_CONFIG="${SYSTEM_CONFIG}"
TRAIN_RUN_CONFIG="${TRAIN_RUN_CONFIG}"
SPONT_RUN_CONFIG="${SPONT_RUN_CONFIG}"
COND_RUN_CONFIG="${COND_RUN_CONFIG}"
PERT_RUN_CONFIG="${PERT_RUN_CONFIG}"

# --- Step-Specific Parameters ---
# Step 4 (Activations)
ACT_RUN_NAME="${ACT_RUN_NAME}"
# Step 6 (GStats)
GSTATS_NAME="${GSTATS_NAME}"
GSTATS_NAMESPACE="${GSTATS_NAMESPACE}"
EOF
# --- *** Finish saving parameters *** ---


echo "All Slurm log files will be saved in: ${LOG_DIR}"
echo "----------------------------------------------------"
echo "Submitting workflow with $PATTERNS patterns..."
echo "----------------------------------------------------"


# ===================================================
#                 JOB SUBMISSION CHAIN
# ===================================================

# 這個變數將持有"下一個"作業所需要的依賴字串
DEP_STRING=""

# --- 步驟 1: genconn ---
if [ $START_STEP -le 1 ] && [ $END_STEP -ge 1 ]; then
    JOB1_ID=$(sbatch \
        --job-name=step1_genconn \
        --output="${LOG_DIR}/step1_genconn_%j.out" \
        --error="${LOG_DIR}/step1_genconn_%j.err" \
        slurm_scripts/run_genconn.sh $NETWORK_CONFIG $PATTERNS | awk '{print $4}')
    echo "Step 1 (genconn) submitted with Job ID: $JOB1_ID"
    # 設定給下一個步驟的依賴
    DEP_STRING="--dependency=afterok:$JOB1_ID"
fi

# --- 步驟 2: train ---
if [ $START_STEP -le 2 ] && [ $END_STEP -ge 2 ]; then
    # 如果 START_STEP > 1, 則 DEP_STRING 為空, 此作業獨立提交
    JOB2_ID=$(sbatch \
        $DEP_STRING \
        --job-name=step2_train \
        --output="${LOG_DIR}/step2_train_%j.out" \
        --error="${LOG_DIR}/step2_train_%j.err" \
        slurm_scripts/run_simulation.sh $SYSTEM_CONFIG $TRAIN_RUN_CONFIG $PATTERNS | awk '{print $4}')
    echo "Step 2 (train) submitted with Job ID: $JOB2_ID"
    # 更新依賴給下一個步驟
    DEP_STRING="--dependency=afterok:$JOB2_ID"
fi

# --- 步驟 3: spontaneous ---
if [ $START_STEP -le 3 ] && [ $END_STEP -ge 3 ]; then
    JOB3_ID=$(sbatch \
        $DEP_STRING \
        --job-name=step3_spontaneous \
        --output="${LOG_DIR}/step3_spontaneous_%j.out" \
        --error="${LOG_DIR}/step3_spontaneous_%j.err" \
        slurm_scripts/run_simulation.sh $SYSTEM_CONFIG $SPONT_RUN_CONFIG $PATTERNS | awk '{print $4}')
    echo "Step 3 (spontaneous) submitted with Job ID: $JOB3_ID"
    DEP_STRING="--dependency=afterok:$JOB3_ID"
fi

# --- 步驟 4: get_activations ---
if [ $START_STEP -le 4 ] && [ $END_STEP -ge 4 ]; then
    JOB4_ID=$(sbatch \
        $DEP_STRING \
        --job-name=step4_activations \
        --output="${LOG_DIR}/step4_activations_%j.out" \
        --error="${LOG_DIR}/step4_activations_%j.err" \
        slurm_scripts/run_activations.sh $PATTERNS $SYSTEM_NAME $ACT_RUN_NAME | awk '{print $4}')
    echo "Step 4 (get_activations) submitted with Job ID: $JOB4_ID"
    DEP_STRING="--dependency=afterok:$JOB4_ID"
fi

# --- 步驟 5: conductances ---
if [ $START_STEP -le 5 ] && [ $END_STEP -ge 5 ]; then
    JOB5_ID=$(sbatch \
        $DEP_STRING \
        --job-name=step5_conductances \
        --output="${LOG_DIR}/step5_conductances_%j.out" \
        --error="${LOG_DIR}/step5_conductances_%j.err" \
        slurm_scripts/run_simulation.sh $SYSTEM_CONFIG $COND_RUN_CONFIG $PATTERNS | awk '{print $4}')
    echo "Step 5 (conductances) submitted with Job ID: $JOB5_ID"
    DEP_STRING="--dependency=afterok:$JOB5_ID"
fi

# --- 步驟 6: gstats ---
if [ $START_STEP -le 6 ] && [ $END_STEP -ge 6 ]; then
    JOB6_ID=$(sbatch \
        $DEP_STRING \
        --job-name=step6_gstats \
        --output="${LOG_DIR}/step6_gstats_%j.out" \
        --error="${LOG_DIR}/step6_gstats_%j.err" \
        slurm_scripts/run_gstats.sh $GSTATS_NAME $GSTATS_NAMESPACE $PATTERNS | awk '{print $4}')
    echo "Step 6 (gstats) submitted with Job ID: $JOB6_ID"
    DEP_STRING="--dependency=afterok:$JOB6_ID"
fi

# --- 步驟 7: perturbation ---
if [ $START_STEP -le 7 ] && [ $END_STEP -ge 7 ]; then
    JOB7_ID=$(sbatch \
        $DEP_STRING \
        --job-name=step7_perturbation \
        --output="${LOG_DIR}/step7_perturbation_%j.out" \
        --error="${LOG_DIR}/step7_perturbation_%j.err" \
        slurm_scripts/run_simulation.sh $SYSTEM_CONFIG $PERT_RUN_CONFIG $PATTERNS | awk '{print $4}')
    echo "Step 7 (perturbation) submitted with Job ID: $JOB7_ID"
    DEP_STRING="--dependency=afterok:$JOB7_ID"
fi

# --- 步驟 8: linear_sensitivity ---
if [ $START_STEP -le 8 ] && [ $END_STEP -ge 8 ]; then
    JOB8_ID=$(sbatch \
        $DEP_STRING \
        --job-name=step8_linear \
        --output="${LOG_DIR}/step8_linear_%j.out" \
        --error="${LOG_DIR}/step8_linear_%j.err" \
        slurm_scripts/run_linear.sh $SYSTEM_NAME $PATTERNS | awk '{print $4}')
    echo "Step 8 (linear_sensitivity) submitted with Job ID: $JOB8_ID"
    # 最後一個步驟，不需要再設定 DEP_STRING
fi

echo "----------------------------------------------------"
echo "已提交所選的 ${START_STEP} 到 ${END_STEP} 步驟。"