SELECT datetime((strftime('%s', m.time_stamp) / ( 60 * 60)) * 60 * 60, 'unixepoch') h, COUNT(*) c, AVG(m.value) a
FROM devices d 
INNER JOIN measurements m on d.id = m.device
WHERE d."type" = "Fuktighetssensor" 
AND d.room = 4
AND DATE(m.time_stamp) = DATE("2023-02-14")
AND m.value > (SELECT AVG(m2.value) a FROM devices d2 
	inner join measurements m2 on d2.id = m2.device
	WHERE d2."type" = "Fuktighetssensor" 
	AND d2.room = 4 
	AND DATE(m2.time_stamp) = DATE("2023-02-14"))
GROUP BY h
HAVING c > 2;
