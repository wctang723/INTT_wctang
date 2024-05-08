CREATE TABLE stability_monitor_1 (
	run_id BIGSERIAL PRIMARY KEY,
	run_number INT NOT NULL,
	event_number INT,
	hit_number INT,
	hit_rate REAL,
	updated_by VARCHAR(30),
	update_version INT NOT NULL,
	update_time DATE NOT NULL,
	comment VARCHAR(300)
);

CREATE TABLE stability_monitor_2 (
	run_id BIGSERIAL PRIMARY KEY,
	run_number INT NOT NULL,
	hot_channel_number REAL,
	dead_channel_number REAL,
	cold_channel_number REAL,
	updated_by VARCHAR(30),
	update_version INT NOT NULL,
	update_time DATE NOT NULL,
	comment VARCHAR(300)
);
