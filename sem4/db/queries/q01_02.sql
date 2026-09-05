SELECT DISTINCT
    c.title      AS comp_title,
    u.username   AS user_name,
    r.name       AS user_role,
    df.filename  AS file_name
FROM competition c
    JOIN participation p        ON p.competition_id   = c.competition_id
    JOIN team_member tm         ON tm.team_id         = p.team_id
    JOIN "user" u               ON u.user_id          = tm.user_id
    JOIN role r                 ON r.role_id          = u.role_id
    JOIN submission s           ON s.participation_id = p.participation_id
    JOIN competition_dataset cd ON cd.competition_id  = c.competition_id
    JOIN dataset d              ON d.dataset_id       = cd.dataset_id
    JOIN dataset_file df        ON df.dataset_id      = d.dataset_id
WHERE
    u.username = $1
    AND r.name = $2
    AND d.name = $3
GROUP BY
    c.competition_id, c.title,
    u.user_id, u.username,
    r.role_id, r.name,
    df.file_id, df.filename
HAVING
    COUNT(DISTINCT s.submission_id) >= 3
ORDER BY
    c.title;

-- 'orion7307', 'судья', 'graphs_8'
