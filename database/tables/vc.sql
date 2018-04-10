/* User table to have the password and user detail used for login perpose */

drop table if exists vc_person;

CREATE TABLE `vc_person`
(
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `first_name` varchar(255) DEFAULT NULL,
  `last_name` varchar(255) DEFAULT NULL,
  `user_name` varchar(255) DEFAULT NULL,
  `phone_no` varchar(50) DEFAULT NULL,
  `status` varchar(255) DEFAULT NULL,
  `is_instructor` varchar(1) DEFAULT 'N',
  `is_admin` varchar(1) DEFAULT 'N',
  `email` varchar(255) DEFAULT NULL,
  `address` varchar(255) DEFAULT NULL,
  `time_zone` varchar(255) DEFAULT NULL,
  `preferred_language` varchar(255) DEFAULT NULL,
  `created_by` text,
  `created_on` datetime DEFAULT NULL,
  `updated_by` text,
  `updated_on` datetime DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Stores person data.';

DROP TABLE IF EXISTS `vc_users`;

CREATE TABLE `vc_users` (
  `id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Primary Key: Unique user ID.',
  `name` varchar(60) NOT NULL DEFAULT '' COMMENT 'Unique user name.',
  `pass` varchar(128) NOT NULL DEFAULT '' COMMENT 'User�s password (hashed).',
  `mail` varchar(254) DEFAULT '' COMMENT 'User�s e-mail address.',
  `created` int(11) NOT NULL DEFAULT '0' COMMENT 'Timestamp for when user was created.',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT 'Whether the user is active(1) or blocked(0).',
  `picture` int(11) NOT NULL DEFAULT '0' COMMENT 'Foreign key: file_managed.fid of user�s picture.',
  PRIMARY KEY (`uid`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Stores user data.';

/*Data for the table `users` */

insert  into `vc_users`(`id`,`name`,`pass`,`mail`,`created`,`status`,`picture`) values (1,'admin','welcome','vc@vc.com','','1',0);

/* Course table for storing courses */

DROP TABLE IF EXISTS `vc_course`;

CREATE TABLE `vc_course` (
  `id` int(11) NOT NULL auto_increment,
  `title` longtext,
  `code` varchar(255) NOT NULL,
  `short_description` longtext,
  `status` varchar(100) default NULL,
  `capacity_min` int(5) default NULL,
  `capacity_max` int(5) default NULL,
  `created_by` text,
  `created_on` datetime default NULL,
  `updated_by` text,
  `updated_on` datetime default NULL,
  PRIMARY KEY kid(`id`)
);

/* session table having the session detail of the course */

DROP TABLE IF EXISTS `vc_session`;

CREATE TABLE `vc_session` (
  `id` int(11) NOT NULL auto_increment,
  `course_id` int(11) default NULL,
  `title` longtext,
  `start_date` datetime default NULL,
  `end_date` datetime default NULL,
  `start_time` varchar(5),
  `end_time` varchar(5),
  `meeting_id` int(11) default NULL,
  `facility_id` int(11) NOT NULL,
  `timezone` varchar(500) default NULL,
  `created_by` text,
  `created_on` datetime default NULL,
  `updated_by` text,
  `updated_on` datetime default NULL,
  PRIMARY KEY  (`id`)
);

/* Content which is attached to the course like pdf/ppt/png , etc */

DROP TABLE IF EXISTS `vc_content`;

CREATE TABLE `vc_content` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `course_id` int(11) NOT NULL,
  `entity_type` varchar(500) DEFAULT NULL,
  `reading_content` text,
  `content_url` CHAR( 1 ) NOT NULL DEFAULT 'N',
  `created_by` text,
  `created_on` datetime DEFAULT NULL,
  `updated_by` text,
  `updated_on` datetime DEFAULT NULL,
  PRIMARY KEY (`id`)
);

/* Having the detail of enrollment of the user and course */

drop table if exists vc_enrollment;

CREATE TABLE `vc_enrollment` (
  `id` int(11) NOT NULL auto_increment,
  `user_id` int(11),
  `course_id` int(11),
  `meeting_id` int(11),
  `reg_status` varchar(100) default NULL,
  `reg_date` datetime default NULL,
  `comp_status` varchar(100) default NULL,
  `comp_date` datetime default NULL,
  `score` FLOAT(11,2) default NULL,
  `created_by` text,
  `created_on` datetime default NULL,
  `updated_by` text,
  `updated_on` datetime default NULL,
  `comp_by` text,
  `comp_on` datetime default NULL,
  PRIMARY KEY  (`id`)
);

/* Will have the code value for the default paramater */

drop table if exists vc_default_code;

CREATE TABLE `vc_default_code` (
  `id` int(11) NOT NULL auto_increment,
  `code` varchar(255) default NULL,
  `name` longtext,
  `is_active` varchar(1) default NULL,
  `attr1` varchar(500) default NULL,
  `attr2` varchar(500) default NULL,
  `attr3` varchar(500) default NULL,
  `attr4` varchar(500) default NULL,
  `created_by` text,
  `created_on` datetime default NULL,
  `updated_by` text,
  `updated_on` datetime default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY code (code)
);
