SELECT
    m.name                           AS metric_name,
    t.name                           AS team_name,
    d.name                           AS dataset_name,
    COUNT(DISTINCT c.competition_id) AS competition_count
FROM team t
JOIN participation p ON p.team_id = t.team_id
JOIN competition c ON c.competition_id = p.competition_id
JOIN configuration cfg ON cfg.competition_id = c.competition_id
JOIN metric m ON m.metric_id = cfg.metric_id
JOIN competition_dataset cd ON cd.competition_id = c.competition_id
JOIN dataset d ON d.dataset_id = cd.dataset_id
WHERE
    m.name = $1
    AND t.name = $2
    AND d.name = $3
GROUP BY
    m.name,
    t.name,
    d.name;

-- 'accuracy', 'Silent Monkeys 571854', 'tables_9'
