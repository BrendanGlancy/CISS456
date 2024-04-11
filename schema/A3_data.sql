-- Create ICD10S table
CREATE TABLE ICD10S (
    Code VARCHAR(3) PRIMARY KEY,
    Description VARCHAR(255)
);

-- Insert data into ICD10S
INSERT INTO ICD10S (Code, Description) VALUES
('S02', 'Fracture of skull and facial bones'),
('S12', 'Fracture of neck'),
('S22', 'Fracture of rib(s), sternum and thoracic spine'),
('S32', 'Fracture of lumbar spine and pelvis'),
('S42', 'Fracture of shoulder and upper arm'),
('S52', 'Fracture of forearm'),
('S62', 'Fracture at wrist and hand level'),
('S72', 'Fracture of femur'),
('S82', 'Fracture of lower leg, including ankle'),
('S92', 'Fracture of foot, except ankle');

-- Create PATIENTS table
CREATE TABLE PATIENTS (
    SSN INT,
    LastName VARCHAR(255),
    Position VARCHAR(5),
    LastServiceDate DATE
);

-- Insert data into PATIENTS
INSERT INTO PATIENTS (SSN, LastName, Position) VALUES
(190000000, 'KOSAR', 'QB'),
(440000000, 'BYNER', 'RB'),
(340000000, 'MACK', 'RB'),
(840000000, 'SLAUGHTER', 'WR'),
(880000000, 'LANGHORNE', 'WR'),
(920000000, 'NEWSOME', 'TE'),
(870000000, 'TUCKER', 'TE'),
(610000000, 'BABB', 'C'),
(770000000, 'BOLDEN', 'OT'),
(630000000, 'RISEN', 'OT'),
(690000000, 'FIKE', 'RG'),
(700000000, 'WILLIAMS', 'LG'),
(960000000, 'CAMP', 'DE'),
(790000000, 'GOLIC', 'DT'),
(900000000, 'SIMS', 'DE'),
(780000000, 'HAIRSTON', 'DE'),
(560000000, 'GRAYSON', 'LB'),
(570000000, 'MATTHEWS', 'LB'),
(290000000, 'DIXON', 'DB'),
(310000000, 'MINNIFIELD', 'CB'),
(220000000, 'WRIGHT', 'FS'),
(240000000, 'ELLIS', 'SS'),
(900000000, 'BAHR', 'K'),
(110000000, 'JOHNSON', 'P');
