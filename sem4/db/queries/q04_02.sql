SELECT
    u.user_id,
    u.username,
    COUNT(p.participation_id) AS p_count
FROM "user" u
    JOIN team_member tm ON tm.user_id = u.user_id
    JOIN participation p ON p.team_id = tm.team_id
GROUP BY
    u.user_id,
    u.username
HAVING COUNT(p.participation_id) = (
    SELECT MIN(p2_count)
    FROM (
        SELECT COUNT(p2.participation_id) AS p2_count
        FROM team_member tm2
        JOIN participation p2 ON p2.team_id = tm2.team_id
        GROUP BY tm2.user_id
    ) counts
);

