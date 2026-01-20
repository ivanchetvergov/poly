import torch
import torch.nn as nn
from torchvision import models, transforms
from PIL import Image
import logging
import os

logger = logging.getLogger(__name__)

class CVAnalyzer:
    """Analyzer class for image processing. ResNet wrapper"""

    def __init__(self, model_path: str = "models/resnet.pth",
                 classes_path: str = "data/processed/classes.pkl"):
        self.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        self.model = _load_model(model_path)
        self.classes = self._load_classes(classes_path)
        self.preprocess = transforms.Compose([
            transforms.Resize(256),
            transforms.CenterCrop(224),
            transforms.ToTensor(),
            transforms.Normalize(mean=[0.485, 0.456, 0.406],
                                 std=[0.229, 0.224, 0.225]),
        ])
        logger.info("CVAnalyzer initialized.")

    def _load_model(self, model_path: str) -> nn.Module:
        if not os.path.exists(model_path):
            raise FileNotFoundError(f"Model file not found at {model_path}")
        model = models.resnet50(pretrained=False)
        model.fc = nn.Sequential(
            nn.Dropout(0.3),
            nn.Linear(model.fc.in_features, len(self.classes))
        )
        model.load_state_dict(torch.load(model_path, map_location=self.device))
        model.to(self.device)
        model.eval()
        logger.info("Model loaded successfully.")
        return model

    def _load_classes(self, classes_path: str) -> list:
        if not os.path.exists(classes_path):
            from torchvision import datasets
            dummy_dataset = datasets.Food101(root='data/raw/', split='train', download=False)
            classes = dummy_dataset.classes
            with open(classes_path, 'wb') as f:
                pickle.dump(classes, f)
        else:
            with open(classes_path, 'rb') as f:
                classes = pickle.load(f)
        return classes

    def analyze_image(self, image_path: str) -> str:
        """Classify an image and return the predicted class."""
        image = image.convert("RGB")
        input_tensor = self.preprocess(image).unsqueeze(0).to(self.device)
        with torch.no_grad():
            outputs = self.model(input_tensor)
            probs = torch.softmax(outputs, dim=1).cpu().numpy()[0]
        predicted_idx = probs.argmax()
        predicted_class = self.classes[predicted_idx]
        logger.info(f"Predicted food class: {predicted_class}")
        return predicted_class
