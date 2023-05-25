SELECT *
FROM motorValue mv 
INNER JOIN(
  SELECT mv.id, DISTINCT (mv.motorId) as oneofeach
  FROM motorValue mv 
  GROUP BY id
) AS t1 USING(mv.id , oneofeach)