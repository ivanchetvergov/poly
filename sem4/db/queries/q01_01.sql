SELECT DISTINCT
    c.title      AS comp_title,
    u.username   AS user_name,
    r.name       AS user_role,
    df.filename  AS file_name
FROM competition c
    JOIN participation p        ON p.competition_id   = c.competition_id    -- участие
    JOIN team_member tm         ON tm.team_id         = p.team_id           -- участники
    JOIN "user" u               ON u.user_id          = tm.user_id           -- пользователь
    JOIN role r                 ON r.role_id          = u.role_id           -- роль пользователя
    JOIN submission s           ON s.participation_id = p.participation_id  -- отправки (пост)
    JOIN competition_dataset cd ON cd.competition_id  = c.competition_id    -- датасет в соревновании
    JOIN dataset d              ON d.dataset_id       = cd.dataset_id       -- датасет
    JOIN dataset_file df        ON df.dataset_id      = d.dataset_id        -- файлы датасета
WHERE
    u.username = $1 -- пользователь "A"
    AND r.name = $2 -- роль "B"
    AND d.name = $3 -- файл датасета "C"
ORDER BY
    c.title;


--- 'luna1048', 'admin', 'graphs_41'
