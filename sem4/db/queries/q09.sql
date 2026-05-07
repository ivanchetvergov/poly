UPDATE submission
SET metric_value = ROUND(metric_value * 1.1, 6)
WHERE submission_id = (
    SELECT s.submission_id
    FROM submission s
        JOIN participation p ON p.participation_id = s.participation_id
        JOIN team t          ON t.team_id          = p.team_id
        JOIN competition c   ON c.competition_id   = p.competition_id
    WHERE s.metric_value IS NOT NULL
    ORDER BY RANDOM()
    LIMIT 1
)
RETURNING submission_id, metric_value;

-- переписать под один запрос
-- update SELECT
