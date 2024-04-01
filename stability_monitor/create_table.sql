CREATE TABLE (
	run_id BIGSERIAL PRIMARY KEY,
	run_number INT NOT NULL,
	event_number INT,
	hit_number INT,
	hit_rate REAL,
	hot_channel_number REAL,
	dead_channel_number REAL,
	cold_channel_number REAL,
	update_time DATE NOT NULL,
	update_version INT NOT NULL,
	comment VARCHAR(300)
);
