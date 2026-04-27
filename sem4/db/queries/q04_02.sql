SELECT
    u.user_id,
    u.username,
    COUNT(p.participation_id) AS p_count
FROM "user" u
    JOIN participation p ON p.user_id = u.user_id
GROUP BY
    u.user_id,
    u.username
HAVING COUNT(p.participation_id) = (
    SELECT MIN(p2_count)
    FROM (
        SELECT COUNT(participation_id) AS p2_count
        FROM participation
        GROUP BY user_id
    ) counts
);

