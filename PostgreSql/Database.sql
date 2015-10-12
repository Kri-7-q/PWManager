-- ---------------------------------------
-- Create database for password manager
-- ---------------------------------------


\! ECHO "Create Database 'pwmanager' ...";
CREATE DATABASE pwmanager;

\c pwmanager

CREATE TABLE account (
id 			SERIAL PRIMARY KEY,
provider 		TEXT NOT NULL,
username		TEXT NOT NULL,
password		TEXT NULL,
question		TEXT NULL,
answer			TEXT NULL,
definedcharacter	TEXT NULL,
passwordlength		INTEGER NULL,
lastmodify		TIMESTAMP NULL,
CONSTRAINT uq_provider_username UNIQUE (provider,username)
);

\! ECHO "Database and table created.";
