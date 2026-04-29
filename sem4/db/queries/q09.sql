UPDATE submission
SET metric_value = $2
WHERE submission_id = $1;

-- переписать под один запрос
-- update SELECT
