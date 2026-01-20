import os
from transformers import BertModel, BertTokenizer

model_dir = './recipe_bert/'
os.makedirs(model_dir, exist_ok=True)

tokenizer = BertTokenizer.from_pretrained('alexdseo/RecipeBERT')
model = BertModel.from_pretrained('alexdseo/RecipeBERT')

tokenizer.save_pretrained(model_dir)
model.save_pretrained(model_dir)
print(f"RecipeBERT saved in {model_dir}")
