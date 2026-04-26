;;; ============================================================
;;;  ML Algorithm Advisor
;;;  Корень: есть ли таргет (метка)?
;;;  ДА  -> поддерево Supervised Learning
;;;  НЕТ -> поддерево Unsupervised & RL
;;; ============================================================

(deftemplate answer
   (slot id)
   (slot value))

(deffunction ask (?q)
   (printout t ?q " (yes/no): ")
   (return (read)))

;;; ============================================================
;;;  КОРЕНЬ
;;; ============================================================

(defrule start
   (not (answer (id root)))
   =>
   (printout t crlf "=== Экспертная система: выбор алгоритма ML ===" crlf)
   (assert (answer (id root)
                   (value (ask "Есть таргет (метка)?")))))

;;; ============================================================
;;;  ПОДДЕРЕВО 1: SUPERVISED LEARNING (root = yes)
;;;  Первый вопрос: таргет непрерывный?
;;; ============================================================

(defrule sup-continuous
   (answer (id root) (value yes))
   (not (answer (id sup-continuous)))
   =>
   (assert (answer (id sup-continuous)
                   (value (ask "Таргет непрерывный (регрессия)?")))))

;;; ==================== РЕГРЕССИЯ (sup-continuous = yes) ====================

(defrule reg-data-size
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value yes))
   (not (answer (id reg-size)))
   =>
   (assert (answer (id reg-size)
                   (value (ask "Данных много (> 10k)?")))))

;;; --- Много данных ---

(defrule reg-big-tabular
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value yes))
   (answer (id reg-size) (value yes))
   (not (answer (id reg-tabular)))
   =>
   (assert (answer (id reg-tabular)
                   (value (ask "Данные табличные?")))))

(defrule reg-big-tab-cat
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value yes))
   (answer (id reg-size) (value yes))
   (answer (id reg-tabular) (value yes))
   (not (answer (id reg-cat)))
   =>
   (assert (answer (id reg-cat)
                   (value (ask "Есть категориальные признаки?")))))

(defrule rec-catboost-reg
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value yes))
   (answer (id reg-size) (value yes))
   (answer (id reg-tabular) (value yes))
   (answer (id reg-cat) (value yes))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: CatBoost (Regressor)" crlf))

(defrule rec-xgb-reg
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value yes))
   (answer (id reg-size) (value yes))
   (answer (id reg-tabular) (value yes))
   (answer (id reg-cat) (value no))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: XGBoost / LightGBM" crlf))

(defrule reg-big-sequential
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value yes))
   (answer (id reg-size) (value yes))
   (answer (id reg-tabular) (value no))
   (not (answer (id reg-seq)))
   =>
   (assert (answer (id reg-seq)
                   (value (ask "Данные последовательные?")))))

(defrule reg-seq-long
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value yes))
   (answer (id reg-size) (value yes))
   (answer (id reg-tabular) (value no))
   (answer (id reg-seq) (value yes))
   (not (answer (id reg-seq-long)))
   =>
   (assert (answer (id reg-seq-long)
                   (value (ask "Последовательность длинная (> 512)?")))))

(defrule rec-transformer-reg
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value yes))
   (answer (id reg-size) (value yes))
   (answer (id reg-tabular) (value no))
   (answer (id reg-seq) (value yes))
   (answer (id reg-seq-long) (value yes))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: Transformer" crlf))

(defrule rec-lstm-gru
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value yes))
   (answer (id reg-size) (value yes))
   (answer (id reg-tabular) (value no))
   (answer (id reg-seq) (value yes))
   (answer (id reg-seq-long) (value no))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: LSTM / GRU" crlf))

(defrule rec-cnn-reg
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value yes))
   (answer (id reg-size) (value yes))
   (answer (id reg-tabular) (value no))
   (answer (id reg-seq) (value no))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: CNN" crlf))

;;; --- Мало данных ---

(defrule reg-small-linear
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value yes))
   (answer (id reg-size) (value no))
   (not (answer (id reg-linear)))
   =>
   (assert (answer (id reg-linear)
                   (value (ask "Зависимость линейная?")))))

(defrule reg-small-lin-feats
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value yes))
   (answer (id reg-size) (value no))
   (answer (id reg-linear) (value yes))
   (not (answer (id reg-feats)))
   =>
   (assert (answer (id reg-feats)
                   (value (ask "Признаков много (> 50)?")))))

(defrule rec-lasso
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value yes))
   (answer (id reg-size) (value no))
   (answer (id reg-linear) (value yes))
   (answer (id reg-feats) (value yes))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: Lasso / ElasticNet" crlf))

(defrule rec-ridge
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value yes))
   (answer (id reg-size) (value no))
   (answer (id reg-linear) (value yes))
   (answer (id reg-feats) (value no))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: Ridge / Linear Regression" crlf))

(defrule reg-small-nonlin-outliers
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value yes))
   (answer (id reg-size) (value no))
   (answer (id reg-linear) (value no))
   (not (answer (id reg-outliers)))
   =>
   (assert (answer (id reg-outliers)
                   (value (ask "Нужна устойчивость к выбросам?")))))

(defrule rec-rf-reg
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value yes))
   (answer (id reg-size) (value no))
   (answer (id reg-linear) (value no))
   (answer (id reg-outliers) (value yes))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: Random Forest Regressor" crlf))

(defrule rec-svr
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value yes))
   (answer (id reg-size) (value no))
   (answer (id reg-linear) (value no))
   (answer (id reg-outliers) (value no))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: SVR (RBF) / Gradient Boosting Regressor" crlf))

;;; ==================== КЛАССИФИКАЦИЯ (sup-continuous = no) ====================

(defrule cls-finetune
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (not (answer (id cls-finetune)))
   =>
   (assert (answer (id cls-finetune)
                   (value (ask "Задача — файнтюнинг LLM?")))))

;;; --- Файнтюнинг = да ---

(defrule cls-ft-data
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value yes))
   (not (answer (id cls-ft-data)))
   =>
   (assert (answer (id cls-ft-data)
                   (value (ask "Данных для файнтюна мало (< 10k)?")))))

(defrule cls-ft-few-memory
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value yes))
   (answer (id cls-ft-data) (value yes))
   (not (answer (id cls-ft-memory)))
   =>
   (assert (answer (id cls-ft-memory)
                   (value (ask "Нужна эффективность по памяти (GPU ограничен)?")))))

(defrule rec-qlora
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value yes))
   (answer (id cls-ft-data) (value yes))
   (answer (id cls-ft-memory) (value yes))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: QLoRA / LoRA" crlf))

(defrule rec-adapter
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value yes))
   (answer (id cls-ft-data) (value yes))
   (answer (id cls-ft-memory) (value no))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: Adapter Tuning / Prefix Tuning" crlf))

(defrule cls-ft-big-instr
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value yes))
   (answer (id cls-ft-data) (value no))
   (not (answer (id cls-ft-instr)))
   =>
   (assert (answer (id cls-ft-instr)
                   (value (ask "Задача требует следования инструкциям?")))))

(defrule rec-instruction-tuning
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value yes))
   (answer (id cls-ft-data) (value no))
   (answer (id cls-ft-instr) (value yes))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: Instruction Tuning / FLAN-style SFT" crlf))

(defrule rec-full-sft
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value yes))
   (answer (id cls-ft-data) (value no))
   (answer (id cls-ft-instr) (value no))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: Full Fine-Tuning / SFT" crlf))

;;; --- Файнтюнинг = нет ---

(defrule cls-big-tabular
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value no))
   (not (answer (id cls-tabular)))
   =>
   (assert (answer (id cls-tabular)
                   (value (ask "Данных много (> 10k)?")))))

(defrule cls-big-tab-cat
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value no))
   (answer (id cls-tabular) (value yes))
   (not (answer (id cls-cat)))
   =>
   (assert (answer (id cls-cat)
                   (value (ask "Данные табличные?")))))

(defrule cls-big-tab-cat2
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value no))
   (answer (id cls-tabular) (value yes))
   (answer (id cls-cat) (value yes))
   (not (answer (id cls-cat2)))
   =>
   (assert (answer (id cls-cat2)
                   (value (ask "Есть категориальные признаки?")))))

(defrule rec-catboost-cls
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value no))
   (answer (id cls-tabular) (value yes))
   (answer (id cls-cat) (value yes))
   (answer (id cls-cat2) (value yes))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: CatBoost (Classifier)" crlf))

(defrule rec-xgb-cls
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value no))
   (answer (id cls-tabular) (value yes))
   (answer (id cls-cat) (value yes))
   (answer (id cls-cat2) (value no))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: XGBoost / LightGBM" crlf))

(defrule cls-big-text
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value no))
   (answer (id cls-tabular) (value yes))
   (answer (id cls-cat) (value no))
   (not (answer (id cls-text)))
   =>
   (assert (answer (id cls-text)
                   (value (ask "Данные текстовые?")))))

(defrule rec-bert
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value no))
   (answer (id cls-tabular) (value yes))
   (answer (id cls-cat) (value no))
   (answer (id cls-text) (value yes))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: BERT / RoBERTa" crlf))

(defrule rec-vit
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value no))
   (answer (id cls-tabular) (value yes))
   (answer (id cls-cat) (value no))
   (answer (id cls-text) (value no))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: ViT / Swin Transformer" crlf))

(defrule cls-small-linear
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value no))
   (answer (id cls-tabular) (value no))
   (not (answer (id cls-linear)))
   =>
   (assert (answer (id cls-linear)
                   (value (ask "Данные линейно разделимы?")))))

(defrule cls-lin-interp
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value no))
   (answer (id cls-tabular) (value no))
   (answer (id cls-linear) (value yes))
   (not (answer (id cls-interp)))
   =>
   (assert (answer (id cls-interp)
                   (value (ask "Нужна интерпретируемость?")))))

(defrule rec-logreg
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value no))
   (answer (id cls-tabular) (value no))
   (answer (id cls-linear) (value yes))
   (answer (id cls-interp) (value yes))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: Logistic Regression" crlf))

(defrule rec-svm-nb
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value no))
   (answer (id cls-tabular) (value no))
   (answer (id cls-linear) (value yes))
   (answer (id cls-interp) (value no))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: SVM (linear) / Naive Bayes" crlf))

(defrule cls-nonlin-balanced
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value no))
   (answer (id cls-tabular) (value no))
   (answer (id cls-linear) (value no))
   (not (answer (id cls-balanced)))
   =>
   (assert (answer (id cls-balanced)
                   (value (ask "Данные сбалансированы по классам?")))))

(defrule rec-rf-catboost
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value no))
   (answer (id cls-tabular) (value no))
   (answer (id cls-linear) (value no))
   (answer (id cls-balanced) (value yes))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: Random Forest / CatBoost" crlf))

(defrule rec-mlp
   (answer (id root) (value yes))
   (answer (id sup-continuous) (value no))
   (answer (id cls-finetune) (value no))
   (answer (id cls-tabular) (value no))
   (answer (id cls-linear) (value no))
   (answer (id cls-balanced) (value no))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: MLP" crlf))

;;; ============================================================
;;;  ПОДДЕРЕВО 2: UNSUPERVISED & RL (root = no)
;;;  Первый вопрос: есть среда с наградой?
;;; ============================================================

(defrule unsup-reward
   (answer (id root) (value no))
   (not (answer (id unsup-reward)))
   =>
   (assert (answer (id unsup-reward)
                   (value (ask "Есть среда с наградой (RL)?")))))

;;; ==================== RL (unsup-reward = yes) ====================

(defrule rl-reward-model
   (answer (id root) (value no))
   (answer (id unsup-reward) (value yes))
   (not (answer (id rl-reward-model)))
   =>
   (assert (answer (id rl-reward-model)
                   (value (ask "Доступна отдельная модель наград?")))))

(defrule rl-paired
   (answer (id root) (value no))
   (answer (id unsup-reward) (value yes))
   (answer (id rl-reward-model) (value yes))
   (not (answer (id rl-paired)))
   =>
   (assert (answer (id rl-paired)
                   (value (ask "Данные предпочтений парные?")))))

(defrule rec-rlhf
   (answer (id root) (value no))
   (answer (id unsup-reward) (value yes))
   (answer (id rl-reward-model) (value yes))
   (answer (id rl-paired) (value yes))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: RLHF / PPO + Reward Model" crlf))

(defrule rec-rlaif
   (answer (id root) (value no))
   (answer (id unsup-reward) (value yes))
   (answer (id rl-reward-model) (value yes))
   (answer (id rl-paired) (value no))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: RLAIF / Constitutional AI" crlf))

(defrule rec-dpo-grpo
   (answer (id root) (value no))
   (answer (id unsup-reward) (value yes))
   (answer (id rl-reward-model) (value no))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: DPO / GRPO" crlf))

;;; ==================== UNSUPERVISED (unsup-reward = no) ====================

(defrule unsup-pretrain
   (answer (id root) (value no))
   (answer (id unsup-reward) (value no))
   (not (answer (id unsup-pretrain)))
   =>
   (assert (answer (id unsup-pretrain)
                   (value (ask "Цель — предобучение языковой модели?")))))

(defrule unsup-lm-context
   (answer (id root) (value no))
   (answer (id unsup-reward) (value no))
   (answer (id unsup-pretrain) (value yes))
   (not (answer (id unsup-context)))
   =>
   (assert (answer (id unsup-context)
                   (value (ask "Нужно понимание двустороннего контекста?")))))

(defrule rec-bert-pretrain
   (answer (id root) (value no))
   (answer (id unsup-reward) (value no))
   (answer (id unsup-pretrain) (value yes))
   (answer (id unsup-context) (value yes))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: BERT / RoBERTa" crlf))

(defrule rec-gpt-pretrain
   (answer (id root) (value no))
   (answer (id unsup-reward) (value no))
   (answer (id unsup-pretrain) (value yes))
   (answer (id unsup-context) (value no))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: GPT-style / Causal LM" crlf))

(defrule unsup-clusters
   (answer (id root) (value no))
   (answer (id unsup-reward) (value no))
   (answer (id unsup-pretrain) (value no))
   (not (answer (id unsup-clusters)))
   =>
   (assert (answer (id unsup-clusters)
                   (value (ask "Цель — найти группы в данных (кластеризация)?")))))

(defrule unsup-k-known
   (answer (id root) (value no))
   (answer (id unsup-reward) (value no))
   (answer (id unsup-pretrain) (value no))
   (answer (id unsup-clusters) (value yes))
   (not (answer (id unsup-k-known)))
   =>
   (assert (answer (id unsup-k-known)
                   (value (ask "Число кластеров известно заранее?")))))

(defrule rec-kmeans-gmm
   (answer (id root) (value no))
   (answer (id unsup-reward) (value no))
   (answer (id unsup-pretrain) (value no))
   (answer (id unsup-clusters) (value yes))
   (answer (id unsup-k-known) (value yes))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: K-Means / GMM" crlf))

(defrule rec-dbscan
   (answer (id root) (value no))
   (answer (id unsup-reward) (value no))
   (answer (id unsup-pretrain) (value no))
   (answer (id unsup-clusters) (value yes))
   (answer (id unsup-k-known) (value no))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: DBSCAN" crlf))

(defrule unsup-dimred
   (answer (id root) (value no))
   (answer (id unsup-reward) (value no))
   (answer (id unsup-pretrain) (value no))
   (answer (id unsup-clusters) (value no))
   (not (answer (id unsup-dimred)))
   =>
   (assert (answer (id unsup-dimred)
                   (value (ask "Нужно снизить размерность данных?")))))

(defrule unsup-dimred-interp
   (answer (id root) (value no))
   (answer (id unsup-reward) (value no))
   (answer (id unsup-pretrain) (value no))
   (answer (id unsup-clusters) (value no))
   (answer (id unsup-dimred) (value yes))
   (not (answer (id unsup-interp)))
   =>
   (assert (answer (id unsup-interp)
                   (value (ask "Нужна интерпретируемость компонент?")))))

(defrule rec-pca
   (answer (id root) (value no))
   (answer (id unsup-reward) (value no))
   (answer (id unsup-pretrain) (value no))
   (answer (id unsup-clusters) (value no))
   (answer (id unsup-dimred) (value yes))
   (answer (id unsup-interp) (value yes))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: PCA" crlf))

(defrule rec-umap-ae
   (answer (id root) (value no))
   (answer (id unsup-reward) (value no))
   (answer (id unsup-pretrain) (value no))
   (answer (id unsup-clusters) (value no))
   (answer (id unsup-dimred) (value yes))
   (answer (id unsup-interp) (value no))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: UMAP / Autoencoder" crlf))

(defrule rec-tsne-umap
   (answer (id root) (value no))
   (answer (id unsup-reward) (value no))
   (answer (id unsup-pretrain) (value no))
   (answer (id unsup-clusters) (value no))
   (answer (id unsup-dimred) (value no))
   =>
   (printout t crlf ">>> РЕКОМЕНДАЦИЯ: t-SNE / UMAP" crlf))
