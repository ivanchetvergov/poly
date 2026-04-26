SELECT
    m.name                           AS metric_name,
    t.name                           AS team_name,
    d.name                           AS dataset_name,
    COUNT(DISTINCT c.competition_id) AS competition_count
FROM competition c
    JOIN configuration cfg      ON cfg.competition_id = c.competition_id -- конфигурация соревнования
    JOIN metric m               ON m.metric_id        = cfg.metric_id    -- метрика в конфигурации
    JOIN team t                 ON t.competition_id   = c.competition_id -- команда через своё поле
    JOIN team_competition tc    ON tc.team_id         = t.team_id        -- датасет команды
    JOIN dataset d              ON d.dataset_id       = tc.dataset_id    -- датасет
    JOIN competition_dataset cd ON cd.competition_id  = c.competition_id -- проверяем датасет в соревновании
                               AND cd.dataset_id      = tc.dataset_id    -- тот же датасет
WHERE
    m.name = $1      -- метрика "A"
    AND t.name = $2  -- команда "B"
    AND d.name = $3 -- датасет "C"
GROUP BY
    m.name,
    t.name,
    d.name;


