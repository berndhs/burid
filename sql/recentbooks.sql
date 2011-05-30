CREATE TABLE recentbooks (
  "title" TEXT NOT NULL,
  "author" TEXT NOT NULL,
  "lastseen" INT,
  "filename" TEXT NOT NULL,
 UNIQUE ( "title", "author","filename") ON CONFLICT REPLACE
)
;
