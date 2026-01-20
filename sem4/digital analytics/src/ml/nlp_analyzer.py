import torch
from transformers import BertModel, BertTokenizer
import numpy as np
import logging
from src.db.retrieval_manager import RetrievalManager

logger = logging.getLogger(__name__)

class NLPAnalyzer:
    """Analyzer class for food. Bert wrapper with FAISS retrieval."""

    def __init__(self, model_path: str, faiss_index_path: str, texts_path: str):
        self.tokenizer = BertTokenizer.from_pretrained(model_path)
        self.model = BertModel.from_pretrained(model_path)
        self.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        self.model.to(self.device)
        self.retrieval = RetrievalManager(faiss_index_path)
        self.texts = np.load(texts_path)
        logger.info("NLPAnalyzer initialized with RetrievalManager.")

    async def analyze(self, input_data: str, k: int = 5) -> list:
        """Retrieve similar recipes for input food class using BERT embeddings and FAISS."""
        inputs = self.tokenizer(input_data, return_tensors='pt', truncation=True, padding=True, max_length=512).to(self.device)
        with torch.no_grad():
            outputs = self.model(**inputs)
        query_emb = outputs.last_hidden_state.mean(dim=1).cpu().numpy().flatten()
        _, indices = self.retrieval.search(query_emb, k)
        recipes = [self.texts[i] for i in indices[0]]
        logger.info(f"Retrieved {len(recipes)} recipes for {input_data}")
        return recipes
