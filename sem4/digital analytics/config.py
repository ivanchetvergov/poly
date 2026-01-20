from dataclasses import dataclass
from pathlib import Path

@dataclass(frozen=True)
class PathConfig:
    root: Path = Path(__file__).resolve().parent
    raw_dir: Path = root / "data" / "raw"
    processed_dir: Path = root / "data" / "processed"
    model_dir: Path = root / "model"

    food101_dir: Path = raw_dir / "food-101"
    food101_images: Path = food101_dir / "images"
    recipe_nlg_csv: Path = raw_dir / "recipe-nlg" / "RecipeNLG_dataset.csv"

    food101_meta_pkl: Path = processed_dir / "food101_meta.pkl"
    food101_meta_pt: Path = processed_dir / "food101_meta.pt"
    ingredient_mapping_pkl: Path = processed_dir / "ingredient_mapping.pkl"
    ingredient_mapping_pt: Path = processed_dir / "ingredient_mapping.pt"

pathConfig = PathConfig()
