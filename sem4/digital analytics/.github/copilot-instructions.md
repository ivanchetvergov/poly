# Copilot Instructions for Digital Analytics Recommender System

## Project Overview
Building a full-stack recommender system for digital analytics with clear separation of concerns: ML logic, infrastructure/deployment, and web frontend. This guide covers all aspects for AI-assisted development in one place.

## Tech Stack
### ML & Data
- **Python**: NumPy, Pandas, Scikit-learn, CatBoost, PySpark
- **Logging**: MLflow (experiment tracking, model registry)
- **Testing**: Pytest, Great Expectations (data validation)
- **Versioning**: Git, DVC for datasets

### Infrastructure & Deployment
- **Containers**: Docker, multi-stage builds
- **Orchestration**: Kubernetes, Docker Compose
- **IaC**: Terraform, CloudFormation
- **Cloud**: AWS (ECS, Lambda), GCP (Cloud Run)
- **CI/CD**: GitHub Actions, CircleCI
- **Monitoring**: ELK stack, Prometheus/Grafana

### Frontend
- **Framework**: React with TypeScript, Next.js for SSR
- **Styling**: Tailwind CSS, Material-UI
- **State**: Redux/Zustand
- **Data Fetching**: Axios, React Query
- **Testing**: Jest, Cypress (E2E)
- **Build**: Vite/Webpack

## Code Conventions (All Layers)
- **Python**: PEP8, format with black, isort; type hints; mypy friendly. Docstrings in Google/NumPy style.
- **TypeScript**: Strict mode, ESLint, Prettier. Functional components with hooks.
- **Naming**: snake_case for Python, camelCase for TS/JS, PascalCase for classes/components.
- **Architecture**: SOLID principles, SRP. One responsibility per class/module.
- **Comments**: Explain "why", not "what". Code should be self-documenting.

## ML Development Guidelines
### Training & Reproducibility
- Fix seeds: numpy, torch, random, sklearn. Always use `random_state` parameter.
- Data versioning: Require and log `dataset_tag` for all training runs.
- Time-based splits: Train/val/test by timestamp for recsys to prevent leakage.
- Model progression: Start simple (MostPopular → ALS/iALS) then rerankers (CatBoost). Justify neural models with resource analysis.

### Experiments & Logging
- Log to MLflow: params, metrics, artifacts, model signature, sample I/O, git commit hash.
- Tag each run: `dataset_tag`, `model_version`, `seed` for reproducibility.
- Structured logs: JSON format for model events, training progress, resource usage.

### Model Artifacts & Handoff
Generate for each trained model:
- Model file (serialized), indices (Faiss, parquet)
- Metadata JSON: `{model_id, version, type, input_schema, output_schema, metrics, dataset_tag, mlflow_uri, resource_requirements}`
- Endpoint spec: POST /predict with schemas, expected latency, batch size
- Load code snippet

### Explainability
- Integrate SHAP for feature importance in ranker models.
- Output format: `{movie_id, score, explanation:[{feature, value, contribution}]}` (top 3 features).
- Auto-save explanations for top-N predictions.

### Testing
- Unit tests: Feature logic, transformations, metrics implementations.
- Integration tests: End-to-end pipeline on synthetic/sample data.
- Data tests: Schema/null/cardinality checks (prefer Great Expectations).
- CI-ready: Fast minimal tests that catch regressions.

## Infrastructure & Deployment Guidelines
### Containerization
- Use Docker with multi-stage builds for optimization.
- Receive ML artifacts (ZIP/S3 path) + metadata JSON.
- Build containers with model loading, health checks, graceful shutdown.

### Deployment Workflow
1. Receive handoff: Artifacts, metadata, API specs, resource requirements
2. Build & test containers locally
3. Deploy to cloud with load balancers, auto-scaling
4. Integration tests, performance benchmarks
5. Set up monitoring, alerting, automated rollback

### Security & Best Practices
- No hardcoded secrets; use env vars or AWS Secrets Manager.
- Least privilege access policies.
- HTTPS only, sanitize inputs.
- Structured logging for debugging.

## Frontend Development Guidelines
### UI/UX
- Build responsive web app for recommendation display and exploration.
- Mobile-first, accessible (WCAG), performant (lazy loading, code splitting).
- Integrate with backend APIs; display recommendations with explanations.

### Data Visualization
- Use D3.js or Recharts for recommendation explanations.
- Show top features contributing to each recommendation.
- Real-time updates via WebSocket or polling.

### Workflow
1. Receive API specs from backend (schemas, endpoints)
2. Build components: Recommendation lists, explanation cards, user profiles
3. Fetch predictions, render with charts
4. Unit tests, E2E tests, accessibility checks

## Integration & Communication
### Cross-Layer Handoffs
- ML → Infra: Artifacts (models, indices), metadata JSON, deployment specs
- Infra → Frontend: API endpoints, schemas, latency SLAs

### API Contract Example
```
POST /predict
Input: {user_id: int, item_ids: [int], features: {...}}
Output: {predictions: [{item_id, score, explanation}]}
Latency: <100ms (p95), Batch: 1-100 items
```

## Best Practices (All Roles)
- **Clarify before coding**: Ask for sample data paths, S3 buckets, MLflow URIs, resource limits, API contracts, design assets.
- **Propose MVPs**: Start with minimal viable examples on sample data for quick validation.
- **PR format**: Summary (3-5 sentences), code files (with docstrings/tests), offline metrics (if ML), testing instructions, QA checklist.
- **No assumptions**: On dataset formats, resource constraints, or requirements—always confirm.

## Common Patterns
### Model Training Script
```python
def train_model(dataset_tag: str, seed: int = 42):
    np.random.seed(seed)
    # ... training logic
    mlflow.log_param("dataset_tag", dataset_tag)
    mlflow.log_param("seed", seed)
```

### Batch Inference (Production)
```python
# Async/batched scoring with CatBoost/Pandas
def predict_batch(data: pd.DataFrame) -> List[float]:
    return model.predict(data)  # Vectorized
```

### Frontend API Integration
```typescript
const fetchRecommendations = async (userId: number, itemIds: number[]) => {
  const response = await axios.post('/predict', { user_id: userId, item_ids: itemIds });
  return response.data.predictions;
};
```
