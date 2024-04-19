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

-- Insert data into PATIENTS
INSERT INTO PATIENTS (SSN, LastName, Position, StateCode) VALUES
(000000019, 'KOSAR', 'QB', 'OH'),
(000000044, 'BYNER', 'RB', 'OH'),
(000000034, 'MACK', 'RB', 'OH'),
(000000084, 'SLAUGHTER', 'WR', 'OH'),
(000000088, 'LANGHORNE', 'WR', 'OH'),
(000000092, 'NEWSOME', 'TE', 'OH'),
(000000087, 'TUCKER', 'TE', 'OH'),
(000000061, 'BABB', 'C', 'OH'),
(000000077, 'BOLDEN', 'OT', 'OH'),
(000000063, 'RISEN', 'OT', 'OH'),
(000000069, 'FIKE', 'RG', 'OH'),
(000000070, 'WILLIAMS', 'LG', 'OH'),
(000000096, 'CAMP', 'DE', 'OH'),
(000000079, 'GOLIC', 'DT', 'OH'),
(000000090, 'SIMS', 'DE', 'OH'),
(000000078, 'HAIRSTON', 'DE', 'OH'),
(000000056, 'GRAYSON', 'LB', 'OH'),
(000000057, 'MATTHEWS', 'LB', 'OH'),
(000000029, 'DIXON', 'DB', 'OH'),
(000000031, 'MINNIFIELD', 'CB', 'OH'),
(000000022, 'WRIGHT', 'FS', 'OH'),
(000000024, 'ELLIS', 'SS', 'OH'),
(000000009, 'BAHR', 'K', 'OH'),
(000000011, 'JOHNSON', 'P', 'OH');

-- Insert injuries for players
INSERT INTO ENCOUNTERS (PatientSSN, ICD10Code, InjuryDate, Description) VALUES
(000000019, 'S02', '1985-09-14', 'Minor fracture of skull during a game collision.'),
