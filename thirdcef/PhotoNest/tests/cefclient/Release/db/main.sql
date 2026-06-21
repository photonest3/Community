/*
 Navicat Premium Data Transfer

 Source Server         : photo1
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 27/06/2024 08:33:38
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for _arts
-- ----------------------------
DROP TABLE IF EXISTS "_arts";
CREATE TABLE "_arts" (
  "id" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
  "md5sum" integer NOT NULL,
  "title1" text COLLATE NOCASE,
  "title2" text COLLATE NOCASE,
  "photos" text COLLATE NOCASE,
  "type" text COLLATE NOCASE,
  "lastmodified" integer,
  "music" text COLLATE NOCASE
);

-- ----------------------------
-- Records of _arts
-- ----------------------------

-- ----------------------------
-- Table structure for _caddie
-- ----------------------------
DROP TABLE IF EXISTS "_caddie";
CREATE TABLE "_caddie" (
  "user_id" text,
  "element_id" integer NOT NULL,
  PRIMARY KEY ("element_id")
);

-- ----------------------------
-- Records of _caddie
-- ----------------------------

-- ----------------------------
-- Table structure for _categories
-- ----------------------------
DROP TABLE IF EXISTS "_categories";
CREATE TABLE "_categories" (
  "id" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
  "name" text COLLATE NOCASE,
  "comment" text COLLATE NOCASE,
  "dir" text COLLATE NOCASE,
  "rank" integer,
  "root_id" integer,
  "visible" integer,
  "representative_id" integer,
  "full_id" text COLLATE NOCASE,
  "imageorder" text COLLATE NOCASE,
  "lastmodified" integer,
  "addby" integer,
  "files" integer
);

-- ----------------------------
-- Records of _categories
-- ----------------------------

-- ----------------------------
-- Table structure for _config
-- ----------------------------
DROP TABLE IF EXISTS "_config";
CREATE TABLE "_config" (
  "param" text COLLATE NOCASE,
  "value" text COLLATE NOCASE,
  "comment" text COLLATE NOCASE,
  PRIMARY KEY ("param"),
  CONSTRAINT "PK_CONFIG" UNIQUE ("param" ASC)
);

-- ----------------------------
-- Records of _config
-- ----------------------------

-- ----------------------------
-- Table structure for _face
-- ----------------------------
DROP TABLE IF EXISTS "_face";
CREATE TABLE "_face" (
  "id" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
  "name" text COLLATE NOCASE,
  "image_id" integer,
  "merge_id" integer,
  "al" integer
);

-- ----------------------------
-- Records of _face
-- ----------------------------

-- ----------------------------
-- Table structure for _favorites
-- ----------------------------
DROP TABLE IF EXISTS "_favorites";
CREATE TABLE "_favorites" (
  "user_id" text,
  "image_id" integer,
  PRIMARY KEY ("image_id")
);

-- ----------------------------
-- Records of _favorites
-- ----------------------------

-- ----------------------------
-- Table structure for _history
-- ----------------------------
DROP TABLE IF EXISTS "_history";
CREATE TABLE "_history" (
  "id" integer PRIMARY KEY AUTOINCREMENT,
  "lastmodified" integer,
  "user_id" text
);

-- ----------------------------
-- Records of _history
-- ----------------------------

-- ----------------------------
-- Table structure for _image_category
-- ----------------------------
DROP TABLE IF EXISTS "_image_category";
CREATE TABLE "_image_category" (
  "image_id" integer,
  "category_id" integer,
  "rank" integer
);

-- ----------------------------
-- Records of _image_category
-- ----------------------------

-- ----------------------------
-- Table structure for _image_face
-- ----------------------------
DROP TABLE IF EXISTS "_image_face";
CREATE TABLE "_image_face" (
  "id" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
  "image_id" integer,
  "face_id" integer,
  "dict" integer
);

-- ----------------------------
-- Records of _image_face
-- ----------------------------

-- ----------------------------
-- Table structure for _image_tag
-- ----------------------------
DROP TABLE IF EXISTS "_image_tag";
CREATE TABLE "_image_tag" (
  "image_id" integer,
  "tag_id" integer
);

-- ----------------------------
-- Records of _image_tag
-- ----------------------------

-- ----------------------------
-- Table structure for _images
-- ----------------------------
DROP TABLE IF EXISTS "_images";
CREATE TABLE "_images" (
  "id" integer PRIMARY KEY AUTOINCREMENT,
  "file" text COLLATE NOCASE,
  "date_available" integer,
  "date_creation" integer,
  "name" text COLLATE NOCASE,
  "comment" text COLLATE NOCASE,
  "author" text COLLATE NOCASE,
  "hit" integer DEFAULT 0,
  "filesize" integer,
  "width" integer,
  "height" integer,
  "coi" text COLLATE NOCASE,
  "representative_ext" text COLLATE NOCASE,
  "date_available_day" integer,
  "path" text COLLATE NOCASE,
  "storage_category_id" integer,
  "quality" integer,
  "md5sum" integer,
  "added_by" text COLLATE NOCASE,
  "rotation" integer,
  "latitude" real,
  "longitude" real,
  "lastmodified" integer,
  "ext" text COLLATE NOCASE,
  "date_creation_day" integer,
  "ratio" real,
  "duration" integer,
  "iscoi" integer,
  "coi_w" integer,
  "coi_h" integer,
  "coi_lastmodified" integer,
  "coi_quality" integer,
  "coi_filesize" integer,
  "orientation" integer,
  "face" integer DEFAULT 0
);

-- ----------------------------
-- Records of _images
-- ----------------------------

-- ----------------------------
-- Table structure for _imgerr
-- ----------------------------
DROP TABLE IF EXISTS "_imgerr";
CREATE TABLE "_imgerr" (
  "id" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
  "cat_id" integer,
  "lastmodified" integer,
  "path" text COLLATE NOCASE,
  "md5sum" integer,
  "msg" text COLLATE NOCASE
);

-- ----------------------------
-- Records of _imgerr
-- ----------------------------

-- ----------------------------
-- Table structure for _log_viewer
-- ----------------------------
DROP TABLE IF EXISTS "_log_viewer";
CREATE TABLE "_log_viewer" (
  "id" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
  "folder" text COLLATE NOCASE,
  "file" text COLLATE NOCASE,
  "lastmodified" integer
);

-- ----------------------------
-- Records of _log_viewer
-- ----------------------------

-- ----------------------------
-- Table structure for _roots
-- ----------------------------
DROP TABLE IF EXISTS "_roots";
CREATE TABLE "_roots" (
  "id" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
  "root_dir" text COLLATE NOCASE,
  "root_sizes" integer
);

-- ----------------------------
-- Records of _roots
-- ----------------------------

-- ----------------------------
-- Table structure for _tags
-- ----------------------------
DROP TABLE IF EXISTS "_tags";
CREATE TABLE "_tags" (
  "id" integer PRIMARY KEY AUTOINCREMENT,
  "name" text COLLATE NOCASE,
  "lastmodified" integer
);

-- ----------------------------
-- Records of _tags
-- ----------------------------

-- ----------------------------
-- Table structure for _upgrade
-- ----------------------------
DROP TABLE IF EXISTS "_upgrade";
CREATE TABLE "_upgrade" (
  "id" text,
  "applied" integer,
  "description" text,
  PRIMARY KEY ("id")
);

-- ----------------------------
-- Records of _upgrade
-- ----------------------------

-- ----------------------------
-- Table structure for _users
-- ----------------------------
DROP TABLE IF EXISTS "_users";
CREATE TABLE "_users" (
  "id" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
  "username" text COLLATE NOCASE,
  "password" text,
  "hint" text
);

-- ----------------------------
-- Records of _users
-- ----------------------------

-- ----------------------------
-- Table structure for sqlite_sequence
-- ----------------------------
DROP TABLE IF EXISTS "sqlite_sequence";
CREATE TABLE "sqlite_sequence" (
  "name",
  "seq"
);

-- ----------------------------
-- Records of sqlite_sequence
-- ----------------------------
INSERT INTO "sqlite_sequence" VALUES ('_imgerr', 0);
INSERT INTO "sqlite_sequence" VALUES ('_arts', 0);
INSERT INTO "sqlite_sequence" VALUES ('_tags', 0);
INSERT INTO "sqlite_sequence" VALUES ('_history', 0);
INSERT INTO "sqlite_sequence" VALUES ('_log_viewer', 0);
INSERT INTO "sqlite_sequence" VALUES ('_categories', 0);
INSERT INTO "sqlite_sequence" VALUES ('_users', 0);
INSERT INTO "sqlite_sequence" VALUES ('_roots', 0);
INSERT INTO "sqlite_sequence" VALUES ('_images', 0);
INSERT INTO "sqlite_sequence" VALUES ('_image_face', 0);
INSERT INTO "sqlite_sequence" VALUES ('_face', 0);

-- ----------------------------
-- Auto increment value for _arts
-- ----------------------------

-- ----------------------------
-- Auto increment value for _categories
-- ----------------------------

-- ----------------------------
-- Indexes structure for table _categories
-- ----------------------------
CREATE INDEX "idx_categories_dir"
ON "_categories" (
  "dir" ASC
);

-- ----------------------------
-- Auto increment value for _face
-- ----------------------------

-- ----------------------------
-- Auto increment value for _history
-- ----------------------------

-- ----------------------------
-- Indexes structure for table _image_category
-- ----------------------------
CREATE INDEX "idx_image_category_categoryid"
ON "_image_category" (
  "category_id" ASC
);
CREATE INDEX "idx_image_category_imageid"
ON "_image_category" (
  "image_id" ASC
);

-- ----------------------------
-- Auto increment value for _image_face
-- ----------------------------

-- ----------------------------
-- Indexes structure for table _image_tag
-- ----------------------------
CREATE INDEX "idx_image_tag_imageid"
ON "_image_tag" (
  "image_id" ASC
);
CREATE INDEX "idx_image_tag_tagid"
ON "_image_tag" (
  "tag_id" ASC
);

-- ----------------------------
-- Auto increment value for _images
-- ----------------------------

-- ----------------------------
-- Indexes structure for table _images
-- ----------------------------
CREATE INDEX "idx_date_available"
ON "_images" (
  "date_available" ASC
);
CREATE INDEX "idx_date_available_day"
ON "_images" (
  "date_available_day" ASC
);
CREATE INDEX "idx_date_creation_day"
ON "_images" (
  "date_creation_day" ASC
);
CREATE INDEX "idx_images_author"
ON "_images" (
  "author" ASC
);
CREATE INDEX "idx_images_ext"
ON "_images" (
  "ext" ASC
);
CREATE INDEX "idx_images_filesize"
ON "_images" (
  "filesize" ASC
);
CREATE INDEX "idx_images_height"
ON "_images" (
  "height" ASC
);
CREATE INDEX "idx_images_hit"
ON "_images" (
  "hit" ASC
);
CREATE INDEX "idx_images_quality"
ON "_images" (
  "quality" ASC
);
CREATE INDEX "idx_images_ratio"
ON "_images" (
  "ratio" ASC
);
CREATE INDEX "idx_images_width"
ON "_images" (
  "width" ASC
);
CREATE INDEX "idx_storage_category_id"
ON "_images" (
  "storage_category_id" ASC
);

-- ----------------------------
-- Auto increment value for _imgerr
-- ----------------------------

-- ----------------------------
-- Auto increment value for _log_viewer
-- ----------------------------

-- ----------------------------
-- Auto increment value for _roots
-- ----------------------------

-- ----------------------------
-- Auto increment value for _tags
-- ----------------------------

-- ----------------------------
-- Auto increment value for _users
-- ----------------------------

PRAGMA foreign_keys = true;
