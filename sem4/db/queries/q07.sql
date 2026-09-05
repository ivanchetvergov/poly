SELECT
    u.username                       AS user_name
FROM "user" u
WHERE u.user_id NOT IN (
    SELECT DISTINCT tm.user_id
    FROM participation p2
        JOIN team_member tm         ON tm.team_id         = p2.team_id
        JOIN submission s           ON s.participation_id = p2.participation_id
        JOIN competition_dataset cd ON cd.competition_id  = p2.competition_id
        JOIN dataset_file fd        ON fd.dataset_id      = cd.dataset_id
    WHERE fd.filename = $1
)
GROUP BY
    u.user_id,
    u.username;

-- 'valid.csv'
