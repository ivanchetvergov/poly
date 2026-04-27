SELECT
    u.username                       AS user_name
FROM "user" u
WHERE u.user_id NOT IN (
    SELECT DISTINCT p2.user_id
    FROM participation p2
        JOIN submission s           ON s.participation_id = p2.participation_id
        JOIN competition_dataset cd ON cd.competition_id  = p2.competition_id
        JOIN dataset d              ON d.dataset_id       = cd.dataset_id
        JOIN dataset_file fd        ON fd.dataset_id      = d.dataset_id
    WHERE fd.filename = $1
)
GROUP BY
    u.user_id,
    u.username;
