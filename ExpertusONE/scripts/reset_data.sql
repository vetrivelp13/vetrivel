-- /*
-- To reset/clear all the data which created by users
-- and set the database to the initial state.
-- 
-- SCOPE: To reset data after every full test run 
--
-- LIMITATION: Reports, Notification & templates, Profile list items, Block/Module settings
-- and Envrionment settings will not be reverted.
-- 
-- */ 

-- DRUPAL TABLES DATA RESET STARTS -----;

delimiter $$

-- TRUNCATE TABLE actions$$
-- TRUNCATE TABLE advagg_aggregates$$
-- TRUNCATE TABLE advagg_aggregates_hashes$$
-- TRUNCATE TABLE advagg_aggregates_versions$$
-- TRUNCATE TABLE advagg_files$$
-- TRUNCATE TABLE authmap$$
-- TRUNCATE TABLE autologout$$
-- TRUNCATE TABLE batch$$
-- TRUNCATE TABLE bbb_meetings$$
-- TRUNCATE TABLE block$$
-- TRUNCATE TABLE block_custom$$
-- TRUNCATE TABLE block_node_type$$
-- TRUNCATE TABLE block_role$$
-- TRUNCATE TABLE blocked_ips$$
TRUNCATE TABLE cache$$
TRUNCATE TABLE cache_advagg_aggregates$$
TRUNCATE TABLE cache_advagg_info$$
TRUNCATE TABLE cache_block$$
TRUNCATE TABLE cache_bootstrap$$
TRUNCATE TABLE cache_field$$
TRUNCATE TABLE cache_filter$$
TRUNCATE TABLE cache_form$$
TRUNCATE TABLE cache_image$$
TRUNCATE TABLE cache_menu$$
TRUNCATE TABLE cache_page$$
TRUNCATE TABLE cache_path$$
TRUNCATE TABLE cache_update$$
TRUNCATE TABLE cache_views$$
TRUNCATE TABLE cache_views_data$$
-- TRUNCATE TABLE captcha_points$$
-- TRUNCATE TABLE captcha_sessions$$
-- TRUNCATE TABLE ckeditor_input_format$$
-- TRUNCATE TABLE ckeditor_settings$$
-- TRUNCATE TABLE comment$$
-- TRUNCATE TABLE context$$
-- TRUNCATE TABLE ctools_css_cache$$
-- TRUNCATE TABLE ctools_object_cache$$
-- TRUNCATE TABLE date_format_locale$$
-- TRUNCATE TABLE date_format_type$$
-- TRUNCATE TABLE date_formats$$
-- TRUNCATE TABLE field_config$$
-- TRUNCATE TABLE field_config_instance$$
-- TRUNCATE TABLE field_data_body$$
TRUNCATE TABLE field_data_comment_body$$
TRUNCATE TABLE field_data_field_image$$
TRUNCATE TABLE field_data_field_tags$$
TRUNCATE TABLE field_data_group_audience$$
TRUNCATE TABLE field_data_scopes$$
TRUNCATE TABLE field_data_taxonomy_catalog$$
TRUNCATE TABLE field_data_taxonomy_forums$$
TRUNCATE TABLE field_data_uc_catalog_image$$
TRUNCATE TABLE field_data_uc_product_image$$
-- TRUNCATE TABLE field_revision_body$$
TRUNCATE TABLE field_revision_comment_body$$
TRUNCATE TABLE field_revision_field_image$$
TRUNCATE TABLE field_revision_field_tags$$
TRUNCATE TABLE field_revision_group_audience$$
TRUNCATE TABLE field_revision_scopes$$
TRUNCATE TABLE field_revision_taxonomy_catalog$$
TRUNCATE TABLE field_revision_taxonomy_forums$$
TRUNCATE TABLE field_revision_uc_catalog_image$$
TRUNCATE TABLE field_revision_uc_product_image$$
TRUNCATE TABLE file_managed$$
TRUNCATE TABLE file_usage$$
-- TRUNCATE TABLE filter$$
-- TRUNCATE TABLE filter_format$$
TRUNCATE TABLE flood$$
TRUNCATE TABLE forum$$
TRUNCATE TABLE forum_index$$
TRUNCATE TABLE history$$
TRUNCATE TABLE image_effects$$
TRUNCATE TABLE image_styles$$
TRUNCATE TABLE ip_ranges$$
TRUNCATE TABLE languages$$

insert  into `languages`(`language`,`name`,`native`,`direction`,`enabled`,`plurals`,`formula`,`domain`,`prefix`,`weight`,`javascript`) values ('en','English Drupal','English Drupal',0,1,0,'','','en',0,'')$$
insert  into `languages`(`language`,`name`,`native`,`direction`,`enabled`,`plurals`,`formula`,`domain`,`prefix`,`weight`,`javascript`) values ('en-us','English','English',0,1,0,'','','en-us',0,'7J8eCS9pN-Xk3bLtBJ4A9LHVY8i3rIDJXcnIWkJZuOA')$$
insert  into `languages`(`language`,`name`,`native`,`direction`,`enabled`,`plurals`,`formula`,`domain`,`prefix`,`weight`,`javascript`) values ('zh-hans','Chinese, Simplified','简体中文',0,1,0,'','','zh-hans',0,'')$$

TRUNCATE TABLE ldap_servers$$
-- TRUNCATE TABLE locales_source$$
-- TRUNCATE TABLE locales_target$$
-- TRUNCATE TABLE menu_custom$$
-- TRUNCATE TABLE menu_links$$
-- TRUNCATE TABLE menu_per_role$$
-- TRUNCATE TABLE menu_router$$
TRUNCATE TABLE messaging_destination$$
TRUNCATE TABLE messaging_message$$
TRUNCATE TABLE messaging_simple$$
TRUNCATE TABLE node$$

insert  into `node`(`nid`,`vid`,`type`,`language`,`title`,`uid`,`status`,`created`,`changed`,`comment`,`promote`,`sticky`,`tnid`,`translate`) values (1,1,'page','und','About Us',1,1,1311249466,1311249466,1,0,0,0,0)$$
insert  into `node`(`nid`,`vid`,`type`,`language`,`title`,`uid`,`status`,`created`,`changed`,`comment`,`promote`,`sticky`,`tnid`,`translate`) values (2,2,'page','und','Feedback',1,1,1311250021,1311250021,1,0,0,0,0)$$
insert  into `node`(`nid`,`vid`,`type`,`language`,`title`,`uid`,`status`,`created`,`changed`,`comment`,`promote`,`sticky`,`tnid`,`translate`) values (3,3,'page','und','Privacy Policy',1,1,1311250326,1311250326,1,0,0,0,0)$$
insert  into `node`(`nid`,`vid`,`type`,`language`,`title`,`uid`,`status`,`created`,`changed`,`comment`,`promote`,`sticky`,`tnid`,`translate`) values (4,4,'page','und','Trademark',1,1,1311250441,1311250441,1,0,0,0,0)$$
insert  into `node`(`nid`,`vid`,`type`,`language`,`title`,`uid`,`status`,`created`,`changed`,`comment`,`promote`,`sticky`,`tnid`,`translate`) values (5,5,'page','und','Legal Notices',1,1,1311250555,1311250555,1,0,0,0,0)$$

TRUNCATE TABLE node_access$$

insert  into `node_access`(`nid`,`gid`,`realm`,`grant_view`,`grant_update`,`grant_delete`) values (0,0,'all',1,0,0)$$

TRUNCATE TABLE node_comment_statistics$$

insert  into `node_comment_statistics`(`nid`,`cid`,`last_comment_timestamp`,`last_comment_name`,`last_comment_uid`,`comment_count`) values (1,0,1311249466,NULL,1,0)$$
insert  into `node_comment_statistics`(`nid`,`cid`,`last_comment_timestamp`,`last_comment_name`,`last_comment_uid`,`comment_count`) values (2,0,1311250021,NULL,1,0)$$
insert  into `node_comment_statistics`(`nid`,`cid`,`last_comment_timestamp`,`last_comment_name`,`last_comment_uid`,`comment_count`) values (3,0,1311250326,NULL,1,0)$$
insert  into `node_comment_statistics`(`nid`,`cid`,`last_comment_timestamp`,`last_comment_name`,`last_comment_uid`,`comment_count`) values (4,0,1311250441,NULL,1,0)$$
insert  into `node_comment_statistics`(`nid`,`cid`,`last_comment_timestamp`,`last_comment_name`,`last_comment_uid`,`comment_count`) values (5,0,1311250555,NULL,1,0)$$

TRUNCATE TABLE node_revision$$

insert  into `node_revision`(`nid`,`vid`,`uid`,`title`,`log`,`timestamp`,`status`,`comment`,`promote`,`sticky`) values (1,1,1,'About Us','',1311249466,1,1,0,0)$$
insert  into `node_revision`(`nid`,`vid`,`uid`,`title`,`log`,`timestamp`,`status`,`comment`,`promote`,`sticky`) values (2,2,1,'Feedback','',1311250021,1,1,0,0)$$
insert  into `node_revision`(`nid`,`vid`,`uid`,`title`,`log`,`timestamp`,`status`,`comment`,`promote`,`sticky`) values (3,3,1,'Privacy Policy','',1311250326,1,1,0,0)$$
insert  into `node_revision`(`nid`,`vid`,`uid`,`title`,`log`,`timestamp`,`status`,`comment`,`promote`,`sticky`) values (4,4,1,'Trademark','',1311250441,1,1,0,0)$$
insert  into `node_revision`(`nid`,`vid`,`uid`,`title`,`log`,`timestamp`,`status`,`comment`,`promote`,`sticky`) values (5,5,1,'Legal Notices','',1311250555,1,1,0,0)$$

TRUNCATE TABLE node_type$$

insert  into `node_type`(`type`,`name`,`base`,`module`,`description`,`help`,`has_title`,`title_label`,`custom`,`modified`,`locked`,`disabled`,`orig_type`) values ('article','Article','node_content','node','Use <em>articles</em> for time-sensitive content like news, press releases or blog posts.','',1,'Title',1,1,0,0,'article')$$
insert  into `node_type`(`type`,`name`,`base`,`module`,`description`,`help`,`has_title`,`title_label`,`custom`,`modified`,`locked`,`disabled`,`orig_type`) values ('forum','Forum topic','forum','forum','A <em>forum topic</em> starts a new discussion thread within a forum.','',1,'Subject',0,0,1,0,'forum')$$
insert  into `node_type`(`type`,`name`,`base`,`module`,`description`,`help`,`has_title`,`title_label`,`custom`,`modified`,`locked`,`disabled`,`orig_type`) values ('meeting','Meeting','node_content','node','ExpertusONE Meeting','',1,'Title',1,1,0,0,'meeting')$$
insert  into `node_type`(`type`,`name`,`base`,`module`,`description`,`help`,`has_title`,`title_label`,`custom`,`modified`,`locked`,`disabled`,`orig_type`) values ('page','Basic page','node_content','node','Use <em>basic pages</em> for your static content, such as an \'About us\' page.','',1,'Title',1,1,0,0,'page')$$
insert  into `node_type`(`type`,`name`,`base`,`module`,`description`,`help`,`has_title`,`title_label`,`custom`,`modified`,`locked`,`disabled`,`orig_type`) values ('product','Product','uc_product','uc_product','Use <em>products</em> to represent items for sale on the website, including all the unique information that can be attributed to a specific model number.','',1,'Name',0,0,1,0,'product')$$
insert  into `node_type`(`type`,`name`,`base`,`module`,`description`,`help`,`has_title`,`title_label`,`custom`,`modified`,`locked`,`disabled`,`orig_type`) values ('product_kit','Product kit','uc_product_kit','uc_product_kit','Use <em>product kits</em> to list two or more products together, presenting a logical and convenient grouping of items to the customer.','',1,'Name',0,0,1,0,'product_kit')$$

TRUNCATE TABLE oauth2_server$$
TRUNCATE TABLE oauth2_server_authorization_code$$
TRUNCATE TABLE oauth2_server_client$$
TRUNCATE TABLE oauth2_server_jti$$
TRUNCATE TABLE oauth2_server_scope$$
TRUNCATE TABLE oauth2_server_token$$
TRUNCATE TABLE oauth_access_tokens$$
TRUNCATE TABLE oauth_authorization_codes$$
TRUNCATE TABLE oauth_clients$$
TRUNCATE TABLE oauth_consumer_registry$$
TRUNCATE TABLE oauth_consumer_token$$
TRUNCATE TABLE oauth_jwt$$
TRUNCATE TABLE oauth_log$$
TRUNCATE TABLE oauth_refresh_tokens$$
TRUNCATE TABLE oauth_scopes$$
TRUNCATE TABLE oauth_server_nonce$$
TRUNCATE TABLE oauth_server_registry$$
TRUNCATE TABLE oauth_server_token$$
TRUNCATE TABLE oauth_users$$
TRUNCATE TABLE og$$
TRUNCATE TABLE og_role$$

insert  into `og_role`(`rid`,`gid`,`name`) values (1,0,'non-member')$$
insert  into `og_role`(`rid`,`gid`,`name`) values (2,0,'member')$$
insert  into `og_role`(`rid`,`gid`,`name`) values (3,0,'administrator member')$$

TRUNCATE TABLE og_role_permission$$

insert  into `og_role_permission`(`rid`,`permission`,`module`) values (3,'administer group','og')$$
insert  into `og_role_permission`(`rid`,`permission`,`module`) values (3,'update group','og')$$

TRUNCATE TABLE og_users_roles$$
TRUNCATE TABLE password_policy$$

insert  into `password_policy`(`pid`,`name`,`description`,`enabled`,`policy`,`created`,`expiration`,`warning`,`weight`) values (1,'Expertus Password Policy','Expertus Password Policy rules for particular roles.',1,'a:1:{s:6:\"length\";s:1:\"6\";}',1315397557,0,'',0)$$

TRUNCATE TABLE password_policy_expiration$$
TRUNCATE TABLE password_policy_force_change$$

insert  into `password_policy_force_change`(`uid`,`force_change`) values (1,0)$$

TRUNCATE TABLE password_policy_history$$
TRUNCATE TABLE password_policy_role$$

insert  into `password_policy_role`(`pid`,`rid`,`name`) values (1,1,'')$$
insert  into `password_policy_role`(`pid`,`rid`,`name`) values (1,2,'')$$
insert  into `password_policy_role`(`pid`,`rid`,`name`) values (1,3,'')$$

TRUNCATE TABLE pm_block_user$$
TRUNCATE TABLE pm_disable$$
TRUNCATE TABLE pm_index$$
TRUNCATE TABLE pm_message$$
TRUNCATE TABLE pm_tags$$

insert  into `pm_tags`(`tag_id`,`tag`,`public`,`hidden`) values (1,'Inbox',0,1)$$

TRUNCATE TABLE pm_tags_index$$
TRUNCATE TABLE queue$$
TRUNCATE TABLE rdf_mapping$$

insert  into `rdf_mapping`(`type`,`bundle`,`mapping`) values ('node','article','a:11:{s:11:\"field_image\";a:2:{s:10:\"predicates\";a:2:{i:0;s:8:\"og:image\";i:1;s:12:\"rdfs:seeAlso\";}s:4:\"type\";s:3:\"rel\";}s:10:\"field_tags\";a:2:{s:10:\"predicates\";a:1:{i:0;s:10:\"dc:subject\";}s:4:\"type\";s:3:\"rel\";}s:7:\"rdftype\";a:2:{i:0;s:9:\"sioc:Item\";i:1;s:13:\"foaf:Document\";}s:5:\"title\";a:1:{s:10:\"predicates\";a:1:{i:0;s:8:\"dc:title\";}}s:7:\"created\";a:3:{s:10:\"predicates\";a:2:{i:0;s:7:\"dc:date\";i:1;s:10:\"dc:created\";}s:8:\"datatype\";s:12:\"xsd:dateTime\";s:8:\"callback\";s:12:\"date_iso8601\";}s:7:\"changed\";a:3:{s:10:\"predicates\";a:1:{i:0;s:11:\"dc:modified\";}s:8:\"datatype\";s:12:\"xsd:dateTime\";s:8:\"callback\";s:12:\"date_iso8601\";}s:4:\"body\";a:1:{s:10:\"predicates\";a:1:{i:0;s:15:\"content:encoded\";}}s:3:\"uid\";a:2:{s:10:\"predicates\";a:1:{i:0;s:16:\"sioc:has_creator\";}s:4:\"type\";s:3:\"rel\";}s:4:\"name\";a:1:{s:10:\"predicates\";a:1:{i:0;s:9:\"foaf:name\";}}s:13:\"comment_count\";a:2:{s:10:\"predicates\";a:1:{i:0;s:16:\"sioc:num_replies\";}s:8:\"datatype\";s:11:\"xsd:integer\";}s:13:\"last_activity\";a:3:{s:10:\"predicates\";a:1:{i:0;s:23:\"sioc:last_activity_date\";}s:8:\"datatype\";s:12:\"xsd:dateTime\";s:8:\"callback\";s:12:\"date_iso8601\";}}')$$
insert  into `rdf_mapping`(`type`,`bundle`,`mapping`) values ('node','forum','a:10:{s:7:\"rdftype\";a:2:{i:0;s:9:\"sioc:Post\";i:1;s:15:\"sioct:BoardPost\";}s:15:\"taxonomy_forums\";a:2:{s:10:\"predicates\";a:1:{i:0;s:18:\"sioc:has_container\";}s:4:\"type\";s:3:\"rel\";}s:5:\"title\";a:1:{s:10:\"predicates\";a:1:{i:0;s:8:\"dc:title\";}}s:7:\"created\";a:3:{s:10:\"predicates\";a:2:{i:0;s:7:\"dc:date\";i:1;s:10:\"dc:created\";}s:8:\"datatype\";s:12:\"xsd:dateTime\";s:8:\"callback\";s:12:\"date_iso8601\";}s:7:\"changed\";a:3:{s:10:\"predicates\";a:1:{i:0;s:11:\"dc:modified\";}s:8:\"datatype\";s:12:\"xsd:dateTime\";s:8:\"callback\";s:12:\"date_iso8601\";}s:4:\"body\";a:1:{s:10:\"predicates\";a:1:{i:0;s:15:\"content:encoded\";}}s:3:\"uid\";a:2:{s:10:\"predicates\";a:1:{i:0;s:16:\"sioc:has_creator\";}s:4:\"type\";s:3:\"rel\";}s:4:\"name\";a:1:{s:10:\"predicates\";a:1:{i:0;s:9:\"foaf:name\";}}s:13:\"comment_count\";a:2:{s:10:\"predicates\";a:1:{i:0;s:16:\"sioc:num_replies\";}s:8:\"datatype\";s:11:\"xsd:integer\";}s:13:\"last_activity\";a:3:{s:10:\"predicates\";a:1:{i:0;s:23:\"sioc:last_activity_date\";}s:8:\"datatype\";s:12:\"xsd:dateTime\";s:8:\"callback\";s:12:\"date_iso8601\";}}')$$
insert  into `rdf_mapping`(`type`,`bundle`,`mapping`) values ('node','page','a:9:{s:7:\"rdftype\";a:1:{i:0;s:13:\"foaf:Document\";}s:5:\"title\";a:1:{s:10:\"predicates\";a:1:{i:0;s:8:\"dc:title\";}}s:7:\"created\";a:3:{s:10:\"predicates\";a:2:{i:0;s:7:\"dc:date\";i:1;s:10:\"dc:created\";}s:8:\"datatype\";s:12:\"xsd:dateTime\";s:8:\"callback\";s:12:\"date_iso8601\";}s:7:\"changed\";a:3:{s:10:\"predicates\";a:1:{i:0;s:11:\"dc:modified\";}s:8:\"datatype\";s:12:\"xsd:dateTime\";s:8:\"callback\";s:12:\"date_iso8601\";}s:4:\"body\";a:1:{s:10:\"predicates\";a:1:{i:0;s:15:\"content:encoded\";}}s:3:\"uid\";a:2:{s:10:\"predicates\";a:1:{i:0;s:16:\"sioc:has_creator\";}s:4:\"type\";s:3:\"rel\";}s:4:\"name\";a:1:{s:10:\"predicates\";a:1:{i:0;s:9:\"foaf:name\";}}s:13:\"comment_count\";a:2:{s:10:\"predicates\";a:1:{i:0;s:16:\"sioc:num_replies\";}s:8:\"datatype\";s:11:\"xsd:integer\";}s:13:\"last_activity\";a:3:{s:10:\"predicates\";a:1:{i:0;s:23:\"sioc:last_activity_date\";}s:8:\"datatype\";s:12:\"xsd:dateTime\";s:8:\"callback\";s:12:\"date_iso8601\";}}')$$
insert  into `rdf_mapping`(`type`,`bundle`,`mapping`) values ('taxonomy_term','forums','a:5:{s:7:\"rdftype\";a:2:{i:0;s:14:\"sioc:Container\";i:1;s:10:\"sioc:Forum\";}s:4:\"name\";a:1:{s:10:\"predicates\";a:2:{i:0;s:10:\"rdfs:label\";i:1;s:14:\"skos:prefLabel\";}}s:11:\"description\";a:1:{s:10:\"predicates\";a:1:{i:0;s:15:\"skos:definition\";}}s:3:\"vid\";a:2:{s:10:\"predicates\";a:1:{i:0;s:13:\"skos:inScheme\";}s:4:\"type\";s:3:\"rel\";}s:6:\"parent\";a:2:{s:10:\"predicates\";a:1:{i:0;s:12:\"skos:broader\";}s:4:\"type\";s:3:\"rel\";}}')$$

-- TRUNCATE TABLE registry$$
-- TRUNCATE TABLE registry_file$$
TRUNCATE TABLE role$$

insert  into `role`(`rid`,`name`,`weight`) values (3,'Administer',16)$$
insert  into `role`(`rid`,`name`,`weight`) values (1,'anonymous user',0)$$
insert  into `role`(`rid`,`name`,`weight`) values (2,'authenticated user',1)$$
insert  into `role`(`rid`,`name`,`weight`) values (5,'Instructor',20)$$
insert  into `role`(`rid`,`name`,`weight`) values (4,'Manager',17)$$

-- TRUNCATE TABLE role_permission$$

DELETE FROM role_permission WHERE rid > 5$$

-- TRUNCATE TABLE search_dataset$$
-- TRUNCATE TABLE search_index$$
-- TRUNCATE TABLE search_node_links$$
-- TRUNCATE TABLE search_total$$
TRUNCATE TABLE semaphore$$
TRUNCATE TABLE sequences$$
TRUNCATE TABLE sessions$$
TRUNCATE TABLE shortcut_set$$
TRUNCATE TABLE shortcut_set_users$$
-- TRUNCATE TABLE system$$
TRUNCATE TABLE taxonomy_index$$
TRUNCATE TABLE taxonomy_term_data$$

insert  into `taxonomy_term_data`(`tid`,`vid`,`name`,`description`,`format`,`weight`) values (1,4,'Any',NULL,NULL,0)$$
insert  into `taxonomy_term_data`(`tid`,`vid`,`name`,`description`,`format`,`weight`) values (2,4,'Root Org-XXXXXXXX1','1',NULL,0)$$
insert  into `taxonomy_term_data`(`tid`,`vid`,`name`,`description`,`format`,`weight`) values (3,3,'--All Delivery types--',NULL,NULL,0)$$
insert  into `taxonomy_term_data`(`tid`,`vid`,`name`,`description`,`format`,`weight`) values (4,5,'--All Tp--',NULL,NULL,0)$$

TRUNCATE TABLE taxonomy_term_hierarchy$$

insert  into `taxonomy_term_hierarchy`(`tid`,`parent`) values (1,0)$$
insert  into `taxonomy_term_hierarchy`(`tid`,`parent`) values (3,0)$$
insert  into `taxonomy_term_hierarchy`(`tid`,`parent`) values (4,0)$$
insert  into `taxonomy_term_hierarchy`(`tid`,`parent`) values (2,1)$$

TRUNCATE TABLE taxonomy_vocabulary$$

insert  into `taxonomy_vocabulary`(`vid`,`name`,`machine_name`,`description`,`hierarchy`,`module`,`weight`) values (1,'Tags','tags','Use tags to group articles on similar topics into categories.',0,'taxonomy',0)$$
insert  into `taxonomy_vocabulary`(`vid`,`name`,`machine_name`,`description`,`hierarchy`,`module`,`weight`) values (2,'Forums','forums','Forum navigation vocabulary',1,'forum',-10)$$
insert  into `taxonomy_vocabulary`(`vid`,`name`,`machine_name`,`description`,`hierarchy`,`module`,`weight`) values (3,'Catalog','catalog','',1,'uc_catalog',0)$$
insert  into `taxonomy_vocabulary`(`vid`,`name`,`machine_name`,`description`,`hierarchy`,`module`,`weight`) values (4,'Organization','organization_nav_vocabulary',NULL,0,'taxonomy',0)$$
insert  into `taxonomy_vocabulary`(`vid`,`name`,`machine_name`,`description`,`hierarchy`,`module`,`weight`) values (5,'Tp Catalog','tp_catalog_nav_vocabulary',NULL,0,'taxonomy',0)$$

TRUNCATE TABLE uc_cart_link_clicks$$
TRUNCATE TABLE uc_cart_products$$
-- TRUNCATE TABLE uc_countries$$
TRUNCATE TABLE uc_discounts$$
TRUNCATE TABLE uc_discounts_classes$$
TRUNCATE TABLE uc_discounts_codes$$
TRUNCATE TABLE uc_discounts_order_codes$$
TRUNCATE TABLE uc_discounts_products$$
TRUNCATE TABLE uc_discounts_roles$$
TRUNCATE TABLE uc_discounts_skus$$
TRUNCATE TABLE uc_discounts_terms$$
TRUNCATE TABLE uc_discounts_uses$$
TRUNCATE TABLE uc_order_admin_comments$$
TRUNCATE TABLE uc_order_comments$$
TRUNCATE TABLE uc_order_line_items$$
TRUNCATE TABLE uc_order_log$$
TRUNCATE TABLE uc_order_products$$
TRUNCATE TABLE uc_order_statuses$$

insert  into `uc_order_statuses`(`order_status_id`,`title`,`state`,`weight`,`locked`) values ('canceled','Canceled','canceled',-20,1)$$
insert  into `uc_order_statuses`(`order_status_id`,`title`,`state`,`weight`,`locked`) values ('completed','Completed','completed',20,1)$$
insert  into `uc_order_statuses`(`order_status_id`,`title`,`state`,`weight`,`locked`) values ('in_checkout','In Checkout','in_checkout',-10,1)$$
insert  into `uc_order_statuses`(`order_status_id`,`title`,`state`,`weight`,`locked`) values ('payment_received','Payment received','payment_received',10,1)$$
insert  into `uc_order_statuses`(`order_status_id`,`title`,`state`,`weight`,`locked`) values ('pending','Pending','post_checkout',0,1)$$
insert  into `uc_order_statuses`(`order_status_id`,`title`,`state`,`weight`,`locked`) values ('processing','Processing','post_checkout',5,1)$$

TRUNCATE TABLE uc_orders$$
TRUNCATE TABLE uc_payment_bank_transfer$$
TRUNCATE TABLE uc_payment_check$$
TRUNCATE TABLE uc_payment_cod$$
TRUNCATE TABLE uc_payment_cybersource_hop_post$$
TRUNCATE TABLE uc_payment_other$$
TRUNCATE TABLE uc_payment_po$$
TRUNCATE TABLE uc_payment_receipts$$
TRUNCATE TABLE uc_product_classes$$
TRUNCATE TABLE uc_product_features$$
TRUNCATE TABLE uc_product_kits$$
TRUNCATE TABLE uc_products$$
-- TRUNCATE TABLE uc_zones$$
TRUNCATE TABLE url_alias$$
TRUNCATE TABLE users$$

insert  into `users`(`uid`,`name`,`pass`,`mail`,`theme`,`signature`,`signature_format`,`created`,`access`,`login`,`status`,`timezone`,`language`,`picture`,`init`,`data`) values (0,'','','expertusone_fulldev@expertus.com','','',NULL,0,0,0,0,NULL,'',0,'',NULL)$$
insert  into `users`(`uid`,`name`,`pass`,`mail`,`theme`,`signature`,`signature_format`,`created`,`access`,`login`,`status`,`timezone`,`language`,`picture`,`init`,`data`) values (1,'admin','$S$C.qQZv.uu5Vsv9McXLQJo9UDZ0QazNExghrwrtc70GhO5SGdgJ58','expertusone_fulldev@expertus.com','','','filtered_html',1309850268,1459415406,1459410292,1,'Asia/Kolkata','en-us',0,'admin@expertusone.com','a:10:{s:25:\"overlay_message_dismissed\";i:1;s:16:\"ckeditor_default\";s:1:\"t\";s:20:\"ckeditor_show_toggle\";s:1:\"t\";s:14:\"ckeditor_popup\";s:1:\"f\";s:13:\"ckeditor_skin\";s:4:\"kama\";s:15:\"ckeditor_expand\";s:1:\"t\";s:14:\"ckeditor_width\";s:4:\"100%\";s:13:\"ckeditor_lang\";s:2:\"en\";s:18:\"ckeditor_auto_lang\";s:1:\"t\";s:7:\"overlay\";i:0;}')$$

TRUNCATE TABLE users_roles$$

insert  into `users_roles`(`uid`,`rid`) values (1,3)$$

-- TRUNCATE TABLE variable$$
TRUNCATE TABLE views_display$$

insert  into `views_display`(`vid`,`id`,`display_title`,`display_plugin`,`position`,`display_options`) values (1,'default','Defaults','default',1,'a:11:{s:5:\"title\";s:6:\"Orders\";s:8:\"use_ajax\";b:1;s:6:\"access\";a:2:{s:4:\"type\";s:4:\"perm\";s:4:\"perm\";s:15:\"view all orders\";}s:5:\"cache\";a:1:{s:4:\"type\";s:4:\"none\";}s:5:\"query\";a:2:{s:4:\"type\";s:11:\"views_query\";s:7:\"options\";a:1:{s:13:\"query_comment\";b:0;}}s:12:\"exposed_form\";a:2:{s:4:\"type\";s:5:\"basic\";s:7:\"options\";a:3:{s:13:\"submit_button\";s:6:\"Search\";s:10:\"autosubmit\";i:0;s:15:\"autosubmit_hide\";i:1;}}s:5:\"pager\";a:2:{s:4:\"type\";s:4:\"full\";s:7:\"options\";a:4:{s:14:\"items_per_page\";s:2:\"10\";s:6:\"offset\";s:1:\"0\";s:2:\"id\";s:1:\"0\";s:6:\"expose\";a:1:{s:26:\"items_per_page_options_all\";i:0;}}}s:12:\"style_plugin\";s:5:\"table\";s:13:\"style_options\";a:6:{s:7:\"columns\";a:6:{s:7:\"actions\";s:7:\"actions\";s:8:\"order_id\";s:8:\"order_id\";s:17:\"billing_full_name\";s:17:\"billing_full_name\";s:12:\"order_status\";s:12:\"order_status\";s:7:\"created\";s:7:\"created\";s:11:\"order_total\";s:11:\"order_total\";}s:7:\"default\";s:8:\"order_id\";s:4:\"info\";a:6:{s:7:\"actions\";a:2:{s:5:\"align\";s:0:\"\";s:9:\"separator\";s:0:\"\";}s:8:\"order_id\";a:3:{s:8:\"sortable\";i:1;s:5:\"align\";s:0:\"\";s:9:\"separator\";s:0:\"\";}s:17:\"billing_full_name\";a:2:{s:5:\"align\";s:0:\"\";s:9:\"separator\";s:0:\"\";}s:12:\"order_status\";a:3:{s:8:\"sortable\";i:1;s:5:\"align\";s:0:\"\";s:9:\"separator\";s:0:\"\";}s:7:\"created\";a:3:{s:8:\"sortable\";i:1;s:5:\"align\";s:0:\"\";s:9:\"separator\";s:0:\"\";}s:11:\"order_total\";a:3:{s:8:\"sortable\";i:1;s:5:\"align\";s:0:\"\";s:9:\"separator\";s:0:\"\";}}s:8:\"override\";i:1;s:6:\"sticky\";i:1;s:5:\"order\";s:4:\"desc\";}s:6:\"fields\";a:6:{s:8:\"order_id\";a:22:{s:2:\"id\";s:8:\"order_id\";s:5:\"table\";s:9:\"uc_orders\";s:5:\"field\";s:8:\"order_id\";s:12:\"relationship\";s:4:\"none\";s:10:\"group_type\";s:5:\"group\";s:7:\"ui_name\";s:0:\"\";s:5:\"label\";s:8:\"Order ID\";s:7:\"exclude\";i:0;s:5:\"alter\";a:22:{s:10:\"alter_text\";i:0;s:4:\"text\";s:0:\"\";s:9:\"make_link\";i:0;s:4:\"path\";s:0:\"\";s:8:\"absolute\";i:0;s:8:\"external\";i:0;s:14:\"replace_spaces\";i:0;s:15:\"trim_whitespace\";i:0;s:3:\"alt\";s:0:\"\";s:3:\"rel\";s:0:\"\";s:10:\"link_class\";s:0:\"\";s:6:\"prefix\";s:0:\"\";s:6:\"suffix\";s:0:\"\";s:6:\"target\";s:0:\"\";s:5:\"nl2br\";i:0;s:10:\"max_length\";s:0:\"\";s:13:\"word_boundary\";i:1;s:8:\"ellipsis\";i:1;s:10:\"strip_tags\";i:0;s:4:\"trim\";i:0;s:13:\"preserve_tags\";s:0:\"\";s:4:\"html\";i:0;}s:12:\"element_type\";s:0:\"\";s:13:\"element_class\";s:0:\"\";s:18:\"element_label_type\";s:0:\"\";s:19:\"element_label_class\";s:0:\"\";s:19:\"element_label_colon\";i:1;s:20:\"element_wrapper_type\";s:0:\"\";s:21:\"element_wrapper_class\";s:0:\"\";s:23:\"element_default_classes\";i:1;s:5:\"empty\";s:0:\"\";s:10:\"hide_empty\";i:0;s:10:\"empty_zero\";i:0;s:16:\"hide_alter_empty\";i:0;s:13:\"link_to_order\";i:0;}s:17:\"billing_full_name\";a:10:{s:2:\"id\";s:17:\"billing_full_name\";s:5:\"table\";s:9:\"uc_orders\";s:5:\"field\";s:17:\"billing_full_name\";s:5:\"label\";s:4:\"User\";s:5:\"alter\";a:12:{s:10:\"alter_text\";i:0;s:9:\"make_link\";i:0;s:8:\"absolute\";i:0;s:8:\"external\";i:0;s:14:\"replace_spaces\";i:0;s:15:\"trim_whitespace\";i:0;s:5:\"nl2br\";i:0;s:13:\"word_boundary\";i:1;s:8:\"ellipsis\";i:1;s:10:\"strip_tags\";i:0;s:4:\"trim\";i:0;s:4:\"html\";i:0;}s:19:\"element_label_colon\";i:1;s:23:\"element_default_classes\";i:1;s:10:\"hide_empty\";i:0;s:10:\"empty_zero\";i:0;s:16:\"hide_alter_empty\";i:0;}s:7:\"created\";a:12:{s:2:\"id\";s:7:\"created\";s:5:\"table\";s:9:\"uc_orders\";s:5:\"field\";s:7:\"created\";s:5:\"label\";s:13:\"Purchase date\";s:5:\"alter\";a:12:{s:10:\"alter_text\";i:0;s:9:\"make_link\";i:0;s:8:\"absolute\";i:0;s:8:\"external\";i:0;s:14:\"replace_spaces\";i:0;s:15:\"trim_whitespace\";i:0;s:5:\"nl2br\";i:0;s:13:\"word_boundary\";i:1;s:8:\"ellipsis\";i:1;s:10:\"strip_tags\";i:0;s:4:\"trim\";i:0;s:4:\"html\";i:0;}s:19:\"element_label_colon\";i:1;s:23:\"element_default_classes\";i:1;s:10:\"hide_empty\";i:0;s:10:\"empty_zero\";i:0;s:16:\"hide_alter_empty\";i:0;s:11:\"date_format\";s:5:\"short\";s:18:\"custom_date_format\";s:17:\"dd/mm/yyyy - H:m \";}s:11:\"order_total\";a:13:{s:2:\"id\";s:11:\"order_total\";s:5:\"table\";s:9:\"uc_orders\";s:5:\"field\";s:11:\"order_total\";s:5:\"label\";s:5:\"Total\";s:5:\"alter\";a:12:{s:10:\"alter_text\";i:0;s:9:\"make_link\";i:0;s:8:\"absolute\";i:0;s:8:\"external\";i:0;s:14:\"replace_spaces\";i:0;s:15:\"trim_whitespace\";i:0;s:5:\"nl2br\";i:0;s:13:\"word_boundary\";i:1;s:8:\"ellipsis\";i:1;s:10:\"strip_tags\";i:0;s:4:\"trim\";i:0;s:4:\"html\";i:0;}s:19:\"element_label_colon\";i:1;s:23:\"element_default_classes\";i:1;s:10:\"hide_empty\";i:0;s:10:\"empty_zero\";i:0;s:16:\"hide_alter_empty\";i:0;s:13:\"set_precision\";i:0;s:9:\"precision\";s:1:\"0\";s:13:\"format_plural\";i:0;}s:12:\"order_status\";a:10:{s:2:\"id\";s:12:\"order_status\";s:5:\"table\";s:9:\"uc_orders\";s:5:\"field\";s:12:\"order_status\";s:5:\"label\";s:6:\"Status\";s:5:\"alter\";a:12:{s:10:\"alter_text\";i:0;s:9:\"make_link\";i:0;s:8:\"absolute\";i:0;s:8:\"external\";i:0;s:14:\"replace_spaces\";i:0;s:15:\"trim_whitespace\";i:0;s:5:\"nl2br\";i:0;s:13:\"word_boundary\";i:1;s:8:\"ellipsis\";i:1;s:10:\"strip_tags\";i:0;s:4:\"trim\";i:0;s:4:\"html\";i:0;}s:19:\"element_label_colon\";i:1;s:23:\"element_default_classes\";i:1;s:10:\"hide_empty\";i:0;s:10:\"empty_zero\";i:0;s:16:\"hide_alter_empty\";i:0;}s:7:\"actions\";a:6:{s:2:\"id\";s:7:\"actions\";s:5:\"table\";s:9:\"uc_orders\";s:5:\"field\";s:7:\"actions\";s:5:\"alter\";a:8:{s:10:\"alter_text\";i:0;s:9:\"make_link\";i:0;s:8:\"absolute\";i:0;s:13:\"word_boundary\";i:1;s:8:\"ellipsis\";i:1;s:10:\"strip_tags\";i:0;s:4:\"trim\";i:0;s:4:\"html\";i:0;}s:10:\"hide_empty\";i:0;s:10:\"empty_zero\";i:0;}}s:7:\"filters\";a:2:{s:8:\"order_id\";a:6:{s:2:\"id\";s:8:\"order_id\";s:5:\"table\";s:9:\"uc_orders\";s:5:\"field\";s:8:\"order_id\";s:5:\"group\";i:0;s:7:\"exposed\";b:1;s:6:\"expose\";a:4:{s:11:\"operator_id\";s:11:\"order_id_op\";s:5:\"label\";s:8:\"Order ID\";s:8:\"operator\";s:11:\"order_id_op\";s:10:\"identifier\";s:8:\"order_id\";}}s:12:\"order_status\";a:6:{s:2:\"id\";s:12:\"order_status\";s:5:\"table\";s:9:\"uc_orders\";s:5:\"field\";s:12:\"order_status\";s:5:\"group\";i:0;s:7:\"exposed\";b:1;s:6:\"expose\";a:6:{s:11:\"operator_id\";s:15:\"order_status_op\";s:5:\"label\";s:12:\"Order Status\";s:8:\"operator\";s:15:\"order_status_op\";s:10:\"identifier\";s:12:\"order_status\";s:8:\"multiple\";i:1;s:6:\"reduce\";i:0;}}}}')$$
insert  into `views_display`(`vid`,`id`,`display_title`,`display_plugin`,`position`,`display_options`) values (1,'page_1','Page','page',2,'a:2:{s:4:\"path\";s:47:\"portalpages/sp_administrator/store-admin/orders\";s:4:\"menu\";a:5:{s:4:\"type\";s:6:\"normal\";s:5:\"title\";s:6:\"Orders\";s:11:\"description\";s:22:\"View and Create Orders\";s:4:\"name\";s:10:\"navigation\";s:6:\"weight\";s:1:\"0\";}}')$$
insert  into `views_display`(`vid`,`id`,`display_title`,`display_plugin`,`position`,`display_options`) values (2,'default','Master','default',1,'a:11:{s:5:\"title\";s:15:\"Commerce Roster\";s:6:\"access\";a:1:{s:4:\"type\";s:4:\"none\";}s:5:\"cache\";a:1:{s:4:\"type\";s:4:\"none\";}s:5:\"query\";a:2:{s:4:\"type\";s:11:\"views_query\";s:7:\"options\";a:1:{s:13:\"query_comment\";b:0;}}s:12:\"exposed_form\";a:1:{s:4:\"type\";s:5:\"basic\";}s:5:\"pager\";a:2:{s:4:\"type\";s:4:\"full\";s:7:\"options\";a:1:{s:14:\"items_per_page\";s:2:\"10\";}}s:12:\"style_plugin\";s:5:\"table\";s:13:\"style_options\";a:6:{s:7:\"columns\";a:6:{s:8:\"order_id\";s:8:\"order_id\";s:17:\"billing_full_name\";s:17:\"billing_full_name\";s:7:\"created\";s:7:\"created\";s:11:\"order_total\";s:11:\"order_total\";s:12:\"order_status\";s:12:\"order_status\";s:7:\"actions\";s:7:\"actions\";}s:7:\"default\";s:2:\"-1\";s:4:\"info\";a:6:{s:8:\"order_id\";a:4:{s:8:\"sortable\";i:0;s:18:\"default_sort_order\";s:3:\"asc\";s:5:\"align\";s:0:\"\";s:9:\"separator\";s:0:\"\";}s:17:\"billing_full_name\";a:2:{s:5:\"align\";s:0:\"\";s:9:\"separator\";s:0:\"\";}s:7:\"created\";a:4:{s:8:\"sortable\";i:0;s:18:\"default_sort_order\";s:3:\"asc\";s:5:\"align\";s:0:\"\";s:9:\"separator\";s:0:\"\";}s:11:\"order_total\";a:4:{s:8:\"sortable\";i:0;s:18:\"default_sort_order\";s:3:\"asc\";s:5:\"align\";s:0:\"\";s:9:\"separator\";s:0:\"\";}s:12:\"order_status\";a:4:{s:8:\"sortable\";i:0;s:18:\"default_sort_order\";s:3:\"asc\";s:5:\"align\";s:0:\"\";s:9:\"separator\";s:0:\"\";}s:7:\"actions\";a:2:{s:5:\"align\";s:0:\"\";s:9:\"separator\";s:0:\"\";}}s:8:\"override\";i:1;s:6:\"sticky\";i:1;s:11:\"empty_table\";i:0;}s:6:\"fields\";a:6:{s:8:\"order_id\";a:3:{s:2:\"id\";s:8:\"order_id\";s:5:\"table\";s:9:\"uc_orders\";s:5:\"field\";s:8:\"order_id\";}s:17:\"billing_full_name\";a:10:{s:2:\"id\";s:17:\"billing_full_name\";s:5:\"table\";s:9:\"uc_orders\";s:5:\"field\";s:17:\"billing_full_name\";s:5:\"label\";s:4:\"User\";s:5:\"alter\";a:12:{s:10:\"alter_text\";i:0;s:9:\"make_link\";i:0;s:8:\"absolute\";i:0;s:8:\"external\";i:0;s:14:\"replace_spaces\";i:0;s:15:\"trim_whitespace\";i:0;s:5:\"nl2br\";i:0;s:13:\"word_boundary\";i:1;s:8:\"ellipsis\";i:1;s:10:\"strip_tags\";i:0;s:4:\"trim\";i:0;s:4:\"html\";i:0;}s:19:\"element_label_colon\";i:1;s:23:\"element_default_classes\";i:1;s:10:\"hide_empty\";i:0;s:10:\"empty_zero\";i:0;s:16:\"hide_alter_empty\";i:0;}s:7:\"created\";a:11:{s:2:\"id\";s:7:\"created\";s:5:\"table\";s:9:\"uc_orders\";s:5:\"field\";s:7:\"created\";s:5:\"label\";s:13:\"Purchase Date\";s:5:\"alter\";a:12:{s:10:\"alter_text\";i:0;s:9:\"make_link\";i:0;s:8:\"absolute\";i:0;s:8:\"external\";i:0;s:14:\"replace_spaces\";i:0;s:15:\"trim_whitespace\";i:0;s:5:\"nl2br\";i:0;s:13:\"word_boundary\";i:1;s:8:\"ellipsis\";i:1;s:10:\"strip_tags\";i:0;s:4:\"trim\";i:0;s:4:\"html\";i:0;}s:19:\"element_label_colon\";i:1;s:23:\"element_default_classes\";i:1;s:10:\"hide_empty\";i:0;s:10:\"empty_zero\";i:0;s:16:\"hide_alter_empty\";i:0;s:11:\"date_format\";s:4:\"long\";}s:11:\"order_total\";a:13:{s:2:\"id\";s:11:\"order_total\";s:5:\"table\";s:9:\"uc_orders\";s:5:\"field\";s:11:\"order_total\";s:5:\"label\";s:5:\"Total\";s:5:\"alter\";a:12:{s:10:\"alter_text\";i:0;s:9:\"make_link\";i:0;s:8:\"absolute\";i:0;s:8:\"external\";i:0;s:14:\"replace_spaces\";i:0;s:15:\"trim_whitespace\";i:0;s:5:\"nl2br\";i:0;s:13:\"word_boundary\";i:1;s:8:\"ellipsis\";i:1;s:10:\"strip_tags\";i:0;s:4:\"trim\";i:0;s:4:\"html\";i:0;}s:19:\"element_label_colon\";i:1;s:23:\"element_default_classes\";i:1;s:10:\"hide_empty\";i:0;s:10:\"empty_zero\";i:0;s:16:\"hide_alter_empty\";i:0;s:13:\"set_precision\";i:0;s:9:\"precision\";s:1:\"0\";s:13:\"format_plural\";i:0;}s:12:\"order_status\";a:9:{s:2:\"id\";s:12:\"order_status\";s:5:\"table\";s:9:\"uc_orders\";s:5:\"field\";s:12:\"order_status\";s:5:\"alter\";a:12:{s:10:\"alter_text\";i:0;s:9:\"make_link\";i:0;s:8:\"absolute\";i:0;s:8:\"external\";i:0;s:14:\"replace_spaces\";i:0;s:15:\"trim_whitespace\";i:0;s:5:\"nl2br\";i:0;s:13:\"word_boundary\";i:1;s:8:\"ellipsis\";i:1;s:10:\"strip_tags\";i:0;s:4:\"trim\";i:0;s:4:\"html\";i:0;}s:19:\"element_label_colon\";i:1;s:23:\"element_default_classes\";i:1;s:10:\"hide_empty\";i:0;s:10:\"empty_zero\";i:0;s:16:\"hide_alter_empty\";i:0;}s:7:\"actions\";a:9:{s:2:\"id\";s:7:\"actions\";s:5:\"table\";s:9:\"uc_orders\";s:5:\"field\";s:7:\"actions\";s:5:\"alter\";a:12:{s:10:\"alter_text\";i:0;s:9:\"make_link\";i:0;s:8:\"absolute\";i:0;s:8:\"external\";i:0;s:14:\"replace_spaces\";i:0;s:15:\"trim_whitespace\";i:0;s:5:\"nl2br\";i:0;s:13:\"word_boundary\";i:1;s:8:\"ellipsis\";i:1;s:10:\"strip_tags\";i:0;s:4:\"trim\";i:0;s:4:\"html\";i:0;}s:19:\"element_label_colon\";i:1;s:23:\"element_default_classes\";i:1;s:10:\"hide_empty\";i:0;s:10:\"empty_zero\";i:0;s:16:\"hide_alter_empty\";i:0;}}s:9:\"arguments\";a:1:{s:3:\"nid\";a:9:{s:2:\"id\";s:3:\"nid\";s:5:\"table\";s:4:\"node\";s:5:\"field\";s:3:\"nid\";s:21:\"default_argument_type\";s:5:\"fixed\";s:25:\"default_argument_skip_url\";i:0;s:7:\"summary\";a:2:{s:17:\"number_of_records\";s:1:\"0\";s:6:\"format\";s:15:\"default_summary\";}s:15:\"summary_options\";a:1:{s:14:\"items_per_page\";s:2:\"25\";}s:12:\"break_phrase\";i:0;s:3:\"not\";i:0;}}s:7:\"filters\";a:3:{s:4:\"name\";a:7:{s:2:\"id\";s:4:\"name\";s:5:\"table\";s:5:\"users\";s:5:\"field\";s:4:\"name\";s:8:\"operator\";s:6:\"starts\";s:5:\"group\";i:0;s:7:\"exposed\";b:1;s:6:\"expose\";a:6:{s:11:\"operator_id\";s:7:\"name_op\";s:5:\"label\";s:9:\"User Name\";s:8:\"operator\";s:7:\"name_op\";s:10:\"identifier\";s:4:\"name\";s:8:\"required\";i:0;s:8:\"multiple\";b:0;}}s:8:\"order_id\";a:6:{s:2:\"id\";s:8:\"order_id\";s:5:\"table\";s:9:\"uc_orders\";s:5:\"field\";s:8:\"order_id\";s:5:\"group\";i:0;s:7:\"exposed\";b:1;s:6:\"expose\";a:5:{s:11:\"operator_id\";s:11:\"order_id_op\";s:5:\"label\";s:8:\"Order ID\";s:8:\"operator\";s:11:\"order_id_op\";s:10:\"identifier\";s:8:\"order_id\";s:8:\"multiple\";b:0;}}s:12:\"order_status\";a:11:{s:2:\"id\";s:12:\"order_status\";s:5:\"table\";s:9:\"uc_orders\";s:5:\"field\";s:12:\"order_status\";s:12:\"relationship\";s:4:\"none\";s:10:\"group_type\";s:5:\"group\";s:7:\"ui_name\";s:0:\"\";s:8:\"operator\";s:2:\"in\";s:5:\"value\";a:3:{s:8:\"canceled\";s:8:\"canceled\";s:7:\"pending\";s:7:\"pending\";s:16:\"payment_received\";s:16:\"payment_received\";}s:5:\"group\";i:0;s:7:\"exposed\";b:1;s:6:\"expose\";a:9:{s:11:\"operator_id\";s:15:\"order_status_op\";s:5:\"label\";s:12:\"Order Status\";s:12:\"use_operator\";i:0;s:8:\"operator\";s:15:\"order_status_op\";s:10:\"identifier\";s:12:\"order_status\";s:8:\"required\";i:0;s:8:\"remember\";i:0;s:8:\"multiple\";i:1;s:6:\"reduce\";i:1;}}}}')$$
insert  into `views_display`(`vid`,`id`,`display_title`,`display_plugin`,`position`,`display_options`) values (2,'page','Page','page',2,'a:1:{s:4:\"path\";s:58:\"portalpages/sp_administrator/store-admin/orders/entities/%\";}')$$

TRUNCATE TABLE views_view$$
TRUNCATE TABLE votingapi_cache$$
TRUNCATE TABLE votingapi_vote$$
TRUNCATE TABLE watchdog$$
TRUNCATE TABLE wysiwyg$$

insert  into `wysiwyg`(`format`,`editor`,`settings`) values ('filtered_html','tinymce',NULL)$$
insert  into `wysiwyg`(`format`,`editor`,`settings`) values ('full_html','tinymce','a:20:{s:7:\"default\";s:1:\"1\";s:11:\"user_choose\";s:1:\"0\";s:11:\"show_toggle\";s:1:\"0\";s:5:\"theme\";s:8:\"advanced\";s:8:\"language\";s:2:\"en\";s:7:\"buttons\";a:1:{s:7:\"default\";a:8:{s:4:\"bold\";i:1;s:6:\"italic\";i:1;s:9:\"underline\";i:1;s:10:\"fontselect\";i:1;s:14:\"fontsizeselect\";i:1;s:9:\"forecolor\";i:1;s:9:\"backcolor\";i:1;s:4:\"code\";i:1;}}s:11:\"toolbar_loc\";s:3:\"top\";s:13:\"toolbar_align\";s:4:\"left\";s:8:\"path_loc\";s:6:\"bottom\";s:8:\"resizing\";s:1:\"1\";s:11:\"verify_html\";s:1:\"1\";s:12:\"preformatted\";s:1:\"0\";s:22:\"convert_fonts_to_spans\";s:1:\"1\";s:17:\"remove_linebreaks\";s:1:\"1\";s:23:\"apply_source_formatting\";s:1:\"0\";s:27:\"paste_auto_cleanup_on_paste\";s:1:\"0\";s:13:\"block_formats\";s:32:\"p,address,pre,h2,h3,h4,h5,h6,div\";s:11:\"css_setting\";s:5:\"theme\";s:8:\"css_path\";s:0:\"\";s:11:\"css_classes\";s:0:\"\";}')$$
insert  into `wysiwyg`(`format`,`editor`,`settings`) values ('plain_text','tinymce',NULL)$$

TRUNCATE TABLE wysiwyg_user$$

-- DRUPAL TABLES DATA RESET ENDS ----$$

-- LMS TABLES DATA RESET STARTS ----$$
TRUNCATE TABLE slt_activity_rating_average$$
TRUNCATE TABLE slt_admincalendar_preference$$
TRUNCATE TABLE slt_aicc_interaction$$
TRUNCATE TABLE slt_announcement_master$$
TRUNCATE TABLE slt_api_log$$
TRUNCATE TABLE slt_attendance_details$$
TRUNCATE TABLE slt_attendance_details_log$$
TRUNCATE TABLE slt_attendance_summary$$
TRUNCATE TABLE slt_attendance_summary_log$$
TRUNCATE TABLE slt_audit_log$$
TRUNCATE TABLE slt_audit_trail$$
TRUNCATE TABLE slt_autoregister_temp$$
TRUNCATE TABLE slt_badge_points$$

INSERT INTO `slt_badge_points`(`id`, `badge_code`, `badge_title`, `badge_points`, `badge_description`, `badge_size`, `badge_path`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (1, 'cre_sys_upt_bg1', 'registration points', 1000, NULL, NULL, NULL, '1', '2013-08-19 15:07:40', NULL, NULL, NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_badge_points`(`id`, `badge_code`, `badge_title`, `badge_points`, `badge_description`, `badge_size`, `badge_path`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (2, 'cre_sys_upt_bg2', 'sharing points', 5000, NULL, NULL, NULL, '1', '2013-08-19 15:07:41', NULL, NULL, NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_badge_points`(`id`, `badge_code`, `badge_title`, `badge_points`, `badge_description`, `badge_size`, `badge_path`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sys_upt_bg3', 'topic/comment/reply points', 10000, NULL, NULL, NULL, '1', '2013-08-19 15:07:41', NULL, NULL, NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_badge_points`(`id`, `badge_code`, `badge_title`, `badge_points`, `badge_description`, `badge_size`, `badge_path`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (4, 'cre_sys_upt_bg4', 'completion points', 15000, NULL, NULL, NULL, '1', '2013-08-19 15:07:41', NULL, NULL, NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_badge_points`(`id`, `badge_code`, `badge_title`, `badge_points`, `badge_description`, `badge_size`, `badge_path`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (5, 'cre_sys_upt_bg5', 'rating points', 20000, NULL, NULL, NULL, '1', '2013-08-19 15:07:41', NULL, NULL, NULL, NULL, NULL, NULL, NULL)$$

TRUNCATE TABLE slt_bulk_notification$$
TRUNCATE TABLE slt_business_rule_mapping$$
-- TRUNCATE TABLE slt_callout$$
TRUNCATE TABLE slt_catalog_access$$
TRUNCATE TABLE slt_cert_curr$$
TRUNCATE TABLE slt_cert_curr_assignment$$
TRUNCATE TABLE slt_cert_curr_path$$
TRUNCATE TABLE slt_cert_curr_pathcatalog$$
TRUNCATE TABLE slt_class_discount$$
TRUNCATE TABLE slt_classroom$$
TRUNCATE TABLE slt_common_mapping$$
TRUNCATE TABLE slt_content_lesson$$
TRUNCATE TABLE slt_content_master$$
TRUNCATE TABLE slt_content_version$$
-- TRUNCATE TABLE slt_country$$
TRUNCATE TABLE slt_course_class$$
TRUNCATE TABLE slt_course_class_session$$
TRUNCATE TABLE slt_course_content_mapper$$
TRUNCATE TABLE slt_course_template$$
TRUNCATE TABLE slt_cstm_salesforce_settings$$
TRUNCATE TABLE slt_currency_conversion_details$$

INSERT INTO `slt_currency_conversion_details` (`base_type`, `convert_type`, `rate`, `created_on`, `updated_on`, `created_by`, `updated_by`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES ('USD', 'USD', '1.0000000000', NOW(), NOW(), 1, 1, NULL, NULL, NULL, NULL, NULL)$$

TRUNCATE TABLE slt_currency_server_logs$$
TRUNCATE TABLE slt_custom_fields$$
TRUNCATE TABLE slt_dataload_batches$$
-- TRUNCATE TABLE slt_dataload_entity$$
TRUNCATE TABLE slt_dataload_jobs$$
TRUNCATE TABLE slt_dataload_process_queue$$
-- TRUNCATE TABLE slt_dataload_table_mapping$$
TRUNCATE TABLE slt_discounts$$
TRUNCATE TABLE slt_drop_policy$$
TRUNCATE TABLE slt_enrollment$$
TRUNCATE TABLE slt_enrollment_content_mapping$$
TRUNCATE TABLE slt_enrollment_exempted$$
TRUNCATE TABLE slt_entity_forum_mapper$$
TRUNCATE TABLE slt_entity_notification_mapping$$
TRUNCATE TABLE slt_entity_profile_mapping$$

INSERT INTO `slt_entity_profile_mapping` (`id`, `entity_id`, `id2`, `entity_type`, `col1`, `col2`, `col3`, `col4`, `col5`, `col6`, `col7`, `col8`, `col9`, `col10`, `col11`, `col12`, `col13`, `col14`, `col15`, `col16`, `col17`, `col18`, `col19`, `col20`, `col21`, `col22`, `col23`, `col24`, `col25`, `col26`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 3, 0, '11', '37327,37336,37333,37334,37338,37339,37330,37331,37342,37554,37555,37346,37347,37349,37350,37351,37352,37353,37461,37355,37357,37359,37534,37505,37506,37553,35447,37365,37366,37368,37369,37370,37371,37372,37373,37528,37529', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '1', '2013-05-30 12:20:50', NULL, NULL, 'NULL', NULL, NULL, NULL, NULL)$$

TRUNCATE TABLE slt_entity_reading_mapper$$
TRUNCATE TABLE slt_equipment$$
TRUNCATE TABLE slt_facility$$
TRUNCATE TABLE slt_ffmpeg_queue$$
TRUNCATE TABLE slt_group_audit$$
TRUNCATE TABLE slt_group_mapping$$
TRUNCATE TABLE slt_group_privilege$$

INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_001', 1, 1, 1, NULL, 1, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_002', 1, 1, 1, NULL, 1, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_003', 1, 1, 1, NULL, 1, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_004', 1, 1, 1, NULL, 1, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_005', 1, 1, 1, NULL, 1, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_cme', 0, 0, 0, NULL, 1, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_frm', 0, 0, 0, NULL, 1, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_nrp', 0, 0, 0, NULL, 1, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_ste', 0, 0, 0, NULL, 1, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_001_001', 1, 1, 1, 'cre_sys_obt_crs', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_001_002', 1, 1, 1, 'cre_sys_obt_trp', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_002_001', 1, 1, 1, 'cre_usr', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_002_002', 1, 1, 1, 'cre_org', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_002_003', 1, 1, 1, 'cre_sec', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_002_004', 0, 0, 0, '', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_003_001', 1, 1, 1, 'sry_det_typ_sry', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_003_002', 1, 1, 1, 'sry_det_typ_sry_qus', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_004_001', 1, 1, 1, 'sry_det_typ_ass', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_004_002', 1, 1, 1, 'sry_det_typ_ass_qus', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_005_001', 1, 1, 1, 'cre_sys_obt_cnt', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_005_002', 0, 0, 0, 'cre_sys_obt_loc', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_005_003', 1, 1, 1, 'cbn_anm_typ_ban', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_005_004', 1, 1, 1, 'cre_ntn', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_005_005', 0, 0, 0, 'cre_cer', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_005_ann', 1, 1, 1, 'cre_sys_obt_not', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_005_api', 1, 1, 1, '', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_cme_001', 0, 0, 0, 'cme_pmt', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_cme_002', 0, 0, 0, 'cme_dis', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_cme_003', 0, 0, 0, '', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_cme_004', 0, 0, 0, '', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_nrp_001', 0, 0, 0, '', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_nrp_002', 0, 0, 0, '', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_nrp_003', 0, 0, 0, '', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_ste_001', 0, 0, 0, '', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_ste_002', 0, 0, 0, '', NULL, '1', '2015-02-02 04:53:51', '1', '2015-02-02 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'cre_sec_pmn_adm_002_003_001', 1, 1, 1, 'cre_sec_learner', NULL, '1', '2015-02-12 04:53:51', '1', '2015-02-12 04:53:51', NULL, NULL, NULL, NULL, NULL)$$
insert into `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) values (1,'cre_sec_pmn_nrp','0','0','0',NULL,'1','1','2015-02-24 06:28:55','1','2015-02-24 06:28:55',NULL,NULL,NULL,NULL,NULL)$$
insert into `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) values (1,'cre_sec_pmn_nrp_001','0','0','0','',NULL,'1','2015-02-24 06:28:55','1','2015-02-24 06:28:55',NULL,NULL,NULL,NULL,NULL)$$
insert into `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) values (1,'cre_sec_pmn_nrp_002','0','0','0','',NULL,'1','2015-02-24 06:28:55','1','2015-02-24 06:28:55',NULL,NULL,NULL,NULL,NULL)$$
insert into `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) values (1,'cre_sec_pmn_nrp_003','0','0','0','',NULL,'1','2015-02-24 06:28:55','1','2015-02-24 06:28:55',NULL,NULL,NULL,NULL,NULL)$$
insert into `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) values (2,'cre_sec_pmn_nrp','0','0','0',NULL,'1','1','2015-02-24 06:28:55','1','2015-02-24 06:28:55',NULL,NULL,NULL,NULL,NULL)$$
insert into `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) values (2,'cre_sec_pmn_nrp_001','0','0','0','',NULL,'1','2015-02-24 06:28:55','1','2015-02-24 06:28:55',NULL,NULL,NULL,NULL,NULL)$$
insert into `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) values (2,'cre_sec_pmn_nrp_002','0','0','0','',NULL,'1','2015-02-24 06:28:55','1','2015-02-24 06:28:55',NULL,NULL,NULL,NULL,NULL)$$
insert into `slt_group_privilege` (`group_id`, `access_page`, `priv_add`, `priv_edit`, `priv_delete`, `page_code`, `is_parent`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) values (2,'cre_sec_pmn_nrp_003','0','0','0','',NULL,'1','2015-02-24 06:28:55','1','2015-02-24 06:28:55',NULL,NULL,NULL,NULL,NULL)$$

TRUNCATE TABLE slt_groups$$

insert  into `slt_groups`(`id`,`code`,`name`,`status`,`access_permissions`,`added_users`,`removed_users`,`userslist`,`duplicate`,`is_admin`,`org_id`,`user_type`,`employment_type`,`country`,`state`,`department`,`job_role`,`language`,`is_manager`,`is_instructor`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (1,'grp_ins','Instructor','cre_sec_sts_atv',NULL,NULL,NULL,NULL,NULL,1,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'N','Y','1','2014-07-12 17:58:40','1','2014-07-12 17:58:40',NULL,NULL,NULL,NULL,NULL)$$
insert  into `slt_groups`(`id`,`code`,`name`,`status`,`access_permissions`,`added_users`,`removed_users`,`userslist`,`duplicate`,`is_admin`,`org_id`,`user_type`,`employment_type`,`country`,`state`,`department`,`job_role`,`language`,`is_manager`,`is_instructor`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (2,'grp_mgr','Manager','cre_sec_sts_atv',NULL,NULL,NULL,NULL,NULL,1,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Y','N','1','2014-07-12 17:58:40','1','2014-07-12 17:58:40',NULL,NULL,NULL,NULL,NULL)$$
insert  into `slt_groups`(`id`,`code`,`name`,`status`,`access_permissions`,`added_users`,`removed_users`,`userslist`,`duplicate`,`is_admin`,`org_id`,`user_type`,`employment_type`,`country`,`state`,`department`,`job_role`,`language`,`is_manager`,`is_instructor`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (3,'grp_adm','Administer','cre_sec_sts_atv','Catalog Course Admin Perm,Training Program Details Admin Perm,Users Admin Perm,Organization Admin Perm,Security Admin Perm,Setup Admin Perm,Survey Details Admin Perm,Survey Questions Admin Perm,Assessment Details Admin Perm,Assessment Questions Admin Perm,Content Admin Perm,Resources Admin Perm,Banner Admin Perm,Notification Admin Perm,Certificate Admin Perm,Announcement Admin Perm,Admin API Perm,configure discounts,view all orders, create orders, edit orders, process credit cards, view cc details, administer order workflow, view own orders, delete orders, administer store order perm,Commerce setup perm,Tax Admin Perm,Module Info Admin Perm,Config Admin Perm,Learning Admin Perm,HR Admin Perm,Survey Admin Perm,Assessment Admin Perm,Manage Admin Perm,Commerce Administer Perm,Site Setup Admin Perm,SmartPortal Admin Perm, Administration Perm,New Report Perm,Create Report Perm,View Report Perm,Assign Report perm',NULL,NULL,NULL,NULL,1,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'N','N','1','2014-07-12 17:58:40','1','2014-07-12 17:58:40',NULL,NULL,NULL,NULL,NULL)$$

TRUNCATE TABLE slt_helppages$$
TRUNCATE TABLE slt_highly_rated_training$$
TRUNCATE TABLE slt_instructor$$
TRUNCATE TABLE slt_ldap_field_mapping$$

insert  into `slt_ldap_field_mapping`(`id`,`entity_type`,`lms_field_name`,`ismandatory`,`ldap_field_name`,`created_on`,`created_by`,`updated_on`,`updated_by`) values (1,'user','FirstName','true','givenname','2011-12-29 18:52:41','admin','2011-12-29 18:52:41','admin')$$
insert  into `slt_ldap_field_mapping`(`id`,`entity_type`,`lms_field_name`,`ismandatory`,`ldap_field_name`,`created_on`,`created_by`,`updated_on`,`updated_by`) values (2,'user','LastName','true','sn','2011-12-29 18:52:41','admin','2011-12-29 18:52:41','admin')$$
insert  into `slt_ldap_field_mapping`(`id`,`entity_type`,`lms_field_name`,`ismandatory`,`ldap_field_name`,`created_on`,`created_by`,`updated_on`,`updated_by`) values (3,'user','Email','true','mail','2011-12-29 18:52:41','admin','2011-12-29 18:52:41','admin')$$
insert  into `slt_ldap_field_mapping`(`id`,`entity_type`,`lms_field_name`,`ismandatory`,`ldap_field_name`,`created_on`,`created_by`,`updated_on`,`updated_by`) values (4,'user','JobTitle','true','title','2011-12-29 18:52:41','admin','2011-12-29 18:52:41','admin')$$
insert  into `slt_ldap_field_mapping`(`id`,`entity_type`,`lms_field_name`,`ismandatory`,`ldap_field_name`,`created_on`,`created_by`,`updated_on`,`updated_by`) values (5,'user','TimeZone','true','extensionattribute1','2011-12-29 18:52:41','admin','2011-12-29 18:52:41','admin')$$
insert  into `slt_ldap_field_mapping`(`id`,`entity_type`,`lms_field_name`,`ismandatory`,`ldap_field_name`,`created_on`,`created_by`,`updated_on`,`updated_by`) values (6,'user','PhoneNo','false','telephonenumber','2011-12-29 18:52:41','admin','2011-12-29 18:52:41','admin')$$

TRUNCATE TABLE slt_learning_request$$
TRUNCATE TABLE slt_location$$
TRUNCATE TABLE slt_manage_pushnotification$$
TRUNCATE TABLE slt_master_enrollment$$
TRUNCATE TABLE slt_master_points$$

INSERT INTO `slt_master_points`(`id`,`code`,`name`,`points`,`created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (1, 'register_class', 'Registering for a Class/Training Plan', 0, '1', '2013-08-19 15:08:18', NULL, NULL, NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_master_points`(`id`,`code`,`name`,`points`,`created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (2, 'sharing_class', 'Sharing a Class', 0, '1', '2013-08-19 15:08:18', NULL, NULL, NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_master_points`(`id`,`code`,`name`,`points`,`created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (3, 'add_topic_comment_reply', 'Adding a Comment/Reply', 0, '1', '2013-08-19 15:08:18', NULL, NULL, NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_master_points`(`id`,`code`,`name`,`points`,`created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (4, 'complete_class_training', 'Completing a Class/Training Plan', 0, '1', '2013-08-19 15:08:18', NULL, NULL, NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_master_points`(`id`,`code`,`name`,`points`,`created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES (5, 'rating_class', 'Rating a Class', 0, '1', '2013-08-19 15:08:18', NULL, NULL, NULL, NULL, NULL, NULL, NULL)$$

TRUNCATE TABLE slt_master_search$$
TRUNCATE TABLE slt_mobile_exchange_contacts$$
TRUNCATE TABLE slt_mobile_ilt_attendance$$
TRUNCATE TABLE slt_mobile_log$$
TRUNCATE TABLE slt_mobiledevice_tokens$$
TRUNCATE TABLE slt_module$$
TRUNCATE TABLE slt_module_crs_mapping$$
-- TRUNCATE TABLE slt_monitoring_jobs$$
TRUNCATE TABLE slt_most_active_users$$
TRUNCATE TABLE slt_most_popular_training$$
TRUNCATE TABLE slt_mro_mapping$$
TRUNCATE TABLE slt_node_learning_activity$$
TRUNCATE TABLE slt_notification$$
-- TRUNCATE TABLE slt_notification_frame$$
-- TRUNCATE TABLE slt_notification_info$$
-- TRUNCATE TABLE slt_notification_keyword$$
TRUNCATE TABLE slt_oauth_access_tokens$$
TRUNCATE TABLE slt_oauth_authorization_codes$$
TRUNCATE TABLE slt_oauth_clients$$
TRUNCATE TABLE slt_oauth_refresh_tokens$$
-- TRUNCATE TABLE slt_oauth_scopes$$
TRUNCATE TABLE slt_object_role_mapping$$
TRUNCATE TABLE slt_order$$
TRUNCATE TABLE slt_order_items$$
TRUNCATE TABLE slt_organization$$
TRUNCATE TABLE slt_payment_error_log$$
TRUNCATE TABLE slt_payment_log$$
TRUNCATE TABLE slt_person$$

insert  into `slt_person`(`id`,`first_name`,`last_name`,`middle_name`,`full_name`,`user_name`,`phone_no`,`status`,`job_role`,`user_type`,`employee_no`,`employment_type`,`job_title`,`hire_date`,`terminated_date`,`is_rehire`,`is_instructor`,`is_manager`,`email`,`manager_id`,`org_id`,`dotted_mngr_id`,`dotted_org_id`,`dept_code`,`addr1`,`addr2`,`city`,`state`,`country`,`zip`,`time_zone`,`preferred_language`,`preferred_loc_id`,`location_name`,`additional_info`,`in_active_on`,`in_active_reason`,`stats_value1`,`stats_value2`,`stats_value3`,`stats_value4`,`stats_value5`,`stats_value6`,`stats_value7`,`stats_value8`,`stats_value9`,`stats_value10`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (1,'ExpertusONE','Admin',NULL,'ExpertusONE Admin','admin','555','cre_usr_sts_atv',NULL,NULL,NULL,NULL,'cre_usr_jtl_eco','2008-11-01 00:00:00',NULL,'N','N','N','expertusone_fulldev1@expertus.com',NULL,NULL,NULL,NULL,NULL,'add1','add2','OFFSHORE','tn','in','6008','cre_sys_tmz_059',NULL,1,'Chennai',NULL,NULL,NULL,3,12,0,0,0,0,2,0,0,0,'Standard Config','2008-10-25 00:55:31','1','2011-04-21 11:37:33','emplo000000000393272','emplo000000000355705','N',NULL,NULL)$$
insert  into `slt_person`(`id`,`first_name`,`last_name`,`middle_name`,`full_name`,`user_name`,`phone_no`,`status`,`job_role`,`user_type`,`employee_no`,`employment_type`,`job_title`,`hire_date`,`terminated_date`,`is_rehire`,`is_instructor`,`is_manager`,`email`,`manager_id`,`org_id`,`dotted_mngr_id`,`dotted_org_id`,`dept_code`,`addr1`,`addr2`,`city`,`state`,`country`,`zip`,`time_zone`,`preferred_language`,`preferred_loc_id`,`location_name`,`additional_info`,`in_active_on`,`in_active_reason`,`stats_value1`,`stats_value2`,`stats_value3`,`stats_value4`,`stats_value5`,`stats_value6`,`stats_value7`,`stats_value8`,`stats_value9`,`stats_value10`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (2,'Guest User','User',NULL,'Guest User User','guest',NULL,'cre_usr_sts_atv',NULL,'Internal',NULL,'Regular','Other','2001-01-01 00:00:00','2001-01-01 00:00:00','N','N','N','expertusone_fulldev2@expertus.com',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'admin','2010-07-26 19:55:18','1','2011-03-18 15:10:20','','','','','2010-09-07 20:09:24#1')$$

TRUNCATE TABLE slt_person_jobrole_mapping$$
TRUNCATE TABLE slt_person_preference$$
TRUNCATE TABLE slt_portable_widgets$$
TRUNCATE TABLE slt_price_baseunit$$
TRUNCATE TABLE slt_price_ppp$$
TRUNCATE TABLE slt_profile_config$$
insert  into `slt_profile_config`(`id`,`code`,`lang_code`,`entity_id`,`category_id`,`profile_field_id`,`profile_value_id`,`display_col`,`type`,`data_type`,`ui`,`cols`,`profile_tag_code`,`flags`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (1,'cre_prf_ctg_cus_sap','cre_sys_lng_eng',1,35474,35891,NULL,'L',NULL,'cre_prf_dty_txt','cre_prf_pui_txb',1,'cre_prf_tag_anm:0,cre_prf_tag_crs:1,cre_prf_tag_cls:0,cre_prf_tag_usr:0,cre_prf_tag_org:0,cre_prf_tag_sec:0,cre_prf_tag_loc:0,cre_prf_tag_cnt:0','','4','2010-07-17 18:39:13','1','2011-04-28 18:06:13',NULL,NULL,NULL,NULL,NULL)$$
insert  into `slt_profile_config`(`id`,`code`,`lang_code`,`entity_id`,`category_id`,`profile_field_id`,`profile_value_id`,`display_col`,`type`,`data_type`,`ui`,`cols`,`profile_tag_code`,`flags`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (7,'cre_prf_ctg_cus_tax','cre_sys_lng_eng',1,35474,35970,35967,'L','LI','cre_prf_dty_ssl','cre_prf_pui_rdb',3,'cre_prf_tag_anm:0,cre_prf_tag_crs:0,cre_prf_tag_cls:0,cre_prf_tag_usr:0,cre_prf_tag_org:1,cre_prf_tag_sec:0,cre_prf_tag_loc:0,cre_prf_tag_cnt:0','','1','2010-08-05 16:05:34','1','2011-04-28 18:06:22',NULL,NULL,NULL,NULL,NULL)$$
insert  into `slt_profile_config`(`id`,`code`,`lang_code`,`entity_id`,`category_id`,`profile_field_id`,`profile_value_id`,`display_col`,`type`,`data_type`,`ui`,`cols`,`profile_tag_code`,`flags`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (8,'cre_prf_ctg_cme_pay','cre_sys_lng_eng',1,35878,35971,35961,'R','LI','cre_prf_dty_ssl','cre_prf_pui_dpn',NULL,'cre_prf_tag_anm:0,cre_prf_tag_crs:0,cre_prf_tag_cls:0,cre_prf_tag_usr:0,cre_prf_tag_org:0,cre_prf_tag_sec:0,cre_prf_tag_loc:1,cre_prf_tag_cnt:0','','1','2010-08-05 16:12:17','1','2011-04-29 11:03:30',NULL,NULL,NULL,NULL,NULL)$$
insert  into `slt_profile_config`(`id`,`code`,`lang_code`,`entity_id`,`category_id`,`profile_field_id`,`profile_value_id`,`display_col`,`type`,`data_type`,`ui`,`cols`,`profile_tag_code`,`flags`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (9,'cre_prf_ctg_cme_ins','cre_sys_lng_eng',1,35878,35972,NULL,'L',NULL,'cre_prf_dty_txt','cre_prf_pui_txa',1,'cre_prf_tag_anm:0,cre_prf_tag_crs:0,cre_prf_tag_cls:0,cre_prf_tag_usr:0,cre_prf_tag_org:0,cre_prf_tag_sec:0,cre_prf_tag_loc:1,cre_prf_tag_cnt:0','','1','2010-08-05 16:12:44','1','2011-04-29 11:03:40',NULL,NULL,NULL,NULL,NULL)$$
insert  into `slt_profile_config`(`id`,`code`,`lang_code`,`entity_id`,`category_id`,`profile_field_id`,`profile_value_id`,`display_col`,`type`,`data_type`,`ui`,`cols`,`profile_tag_code`,`flags`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (14,'cre_prf_ctg_tny_dep','cre_sys_lng_eng',1,35879,35997,35572,'L','LI','cre_prf_dty_hrc','cre_prf_pui_tre',1,'cre_prf_tag_anm:0,cre_prf_tag_crs:1,cre_prf_tag_cls:0,cre_prf_tag_usr:1,cre_prf_tag_org:0,cre_prf_tag_sec:0,cre_prf_tag_loc:0,cre_prf_tag_cnt:0','','1','2010-08-10 14:07:34','1','2011-04-28 12:31:37',NULL,NULL,NULL,NULL,NULL)$$
insert  into `slt_profile_config`(`id`,`code`,`lang_code`,`entity_id`,`category_id`,`profile_field_id`,`profile_value_id`,`display_col`,`type`,`data_type`,`ui`,`cols`,`profile_tag_code`,`flags`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (15,'cre_prf_ctg_cus_reg','cre_sys_lng_eng',1,35474,36064,36058,'L','LI','cre_prf_dty_ssl','cre_prf_pui_dpn',4,'cre_prf_tag_anm:0,cre_prf_tag_crs:0,cre_prf_tag_cls:0,cre_prf_tag_usr:0,cre_prf_tag_org:0,cre_prf_tag_sec:0,cre_prf_tag_loc:1,cre_prf_tag_cnt:0','','1','2010-08-30 02:26:32','1','2011-04-28 18:06:46',NULL,NULL,NULL,NULL,NULL)$$
insert  into `slt_profile_config`(`id`,`code`,`lang_code`,`entity_id`,`category_id`,`profile_field_id`,`profile_value_id`,`display_col`,`type`,`data_type`,`ui`,`cols`,`profile_tag_code`,`flags`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (17,'cre_prf_ctg_cus_ctp','cre_sys_lng_eng',1,35474,37270,37267,'L','LI','cre_prf_dty_ssl','cre_prf_pui_rdb',2,'cre_prf_tag_anm:0,cre_prf_tag_crs:0,cre_prf_tag_cls:1,cre_prf_tag_usr:0,cre_prf_tag_org:0,cre_prf_tag_sec:0,cre_prf_tag_loc:0,cre_prf_tag_cnt:0','','1','2011-04-29 11:22:06','1','2011-04-29 11:22:15',NULL,NULL,NULL,NULL,NULL)$$
insert  into `slt_profile_config`(`id`,`code`,`lang_code`,`entity_id`,`category_id`,`profile_field_id`,`profile_value_id`,`display_col`,`type`,`data_type`,`ui`,`cols`,`profile_tag_code`,`flags`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (19,'cre_prf_ctg_brl_cls','cre_sys_lng_eng',1,37138,37272,37151,'L','LI','cre_prf_dty_msl','cre_prf_pui_chk',2,'cre_prf_tag_anm:0,cre_prf_tag_crs:0,cre_prf_tag_cls:1,cre_prf_tag_usr:0,cre_prf_tag_org:0,cre_prf_tag_sec:0,cre_prf_tag_loc:0,cre_prf_tag_cnt:0','','1','2011-04-29 15:08:58','1','2011-04-29 15:10:14',NULL,NULL,NULL,NULL,NULL)$$
insert  into `slt_profile_config`(`id`,`code`,`lang_code`,`entity_id`,`category_id`,`profile_field_id`,`profile_value_id`,`display_col`,`type`,`data_type`,`ui`,`cols`,`profile_tag_code`,`flags`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (20,'cre_prf_ctg_brl_tpm','cre_sys_lng_eng',1,37138,37273,37144,'L','LI','cre_prf_dty_msl','cre_prf_pui_lst',3,'cre_prf_tag_anm:0,cre_prf_tag_crs:0,cre_prf_tag_cls:0,cre_prf_tag_usr:0,cre_prf_tag_org:0,cre_prf_tag_sec:0,cre_prf_tag_loc:0,cre_prf_tag_cnt:0','','1','2011-04-29 15:09:49',NULL,NULL,NULL,NULL,NULL,NULL,NULL)$$
insert  into `slt_profile_config`(`id`,`code`,`lang_code`,`entity_id`,`category_id`,`profile_field_id`,`profile_value_id`,`display_col`,`type`,`data_type`,`ui`,`cols`,`profile_tag_code`,`flags`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (24,'per001','cre_sys_lng_eng',1,35475,37443,35441,'L','LI','cre_prf_dty_hrc','cre_prf_pui_tre',1,'cre_prf_tag_anm:0,cre_prf_tag_crs:0,cre_prf_tag_cls:0,cre_prf_tag_usr:0,cre_prf_tag_org:0,cre_prf_tag_sec:1,cre_prf_tag_loc:0,cre_prf_tag_cnt:0,cre_prf_tag_lpn:0,cre_prf_tag_crt:0,cre_prf_tag_cur:0','','1','2011-12-29 19:21:34',NULL,NULL,NULL,NULL,NULL,NULL,NULL)$$

-- TRUNCATE TABLE slt_profile_list$$
-- TRUNCATE TABLE slt_profile_list_items$$
-- TRUNCATE TABLE slt_profile_tagging_defn$$
TRUNCATE TABLE slt_program$$
TRUNCATE TABLE slt_promoted_highly_rated$$
TRUNCATE TABLE slt_re_certify$$
TRUNCATE TABLE slt_report_common_themes$$

INSERT INTO `slt_report_common_themes`(`id`,`theme_name`,`header_style`,`header_style_tip`,`footer_style`,`footer_style_tip`,`criteria_column_style`,`criteria_column_style_tip`,`grid_header_style`,`grid_header_font_style`,`grid_header_style_tip`,`grid_row_border_style`,`grid_row_style`,`grid_row_style_tip`,`grid_alternate_row_style`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) VALUES (1,'ExpertusONE Theme','font-family : Arial; font-size : 24px; color : #333300; font-style: Normal;','{\"fontfamily\":\"Arial\",\"fontsize\":\"24px\",\"color\":\"#333300\",\"fontstyle\":\"Normal\"}','font-family : Arial; font-size : 12px; color : #999999; font-style: normal;','{"font-family":"Arial","font-size":"12px","color":"#999999","font-style":"normal"}','font-family : ProximaNovaBold; font-size : 12px; color : #333333; font-style: Normal;','{"font-family":"Arial","font-size":"12px","color":"#333333","font-style":"Normal"}','margin: 0px; padding: 0px; padding-left: 2px; background-color: #ffffff !important; line-height: 20px; border: 1px solid #ffffff !important;','font-family : Arial; font-size : 13px; color : #808080; font-style: Normal;','{\"backgroundcolor\":\"#ffffff\",\"lineheight\":\"20\",\"border\":\"#ffffff\",\"fontfamily\":\"Arial\",\"fontsize\":\"13px\",\"color\":\"#808080\",\"fontstyle\":\"Normal\"}','border-left:1px solid #ffffff; border-right:1px solid #ffffff;','margin: 0px; padding-right: 5px; padding-left: 4px; background-color: #ffffff; line-height: 30px; font-family : Arial !important; font-size : 12px; color : #333333; font-style: Normal; border-bottom:1px solid #ffffff;','{\"backgroundcolor\":\"#ffffff\",\"backgroundaltcolor\":\"#efefef\",\"lineheight\":\"30\",\"fontfamily\":\"Arial\",\"fontsize\":\"12px\",\"color\":\"#333333\",\"fontstyle\":\"Normal\",\"border\":\"#ffffff\"}','margin: 0px; padding-right: 5px; padding-left: 4px; background-color: #efefef; line-height: 30px; font-family : Arial !important; font-size : 12px; color : #333333; font-style: Normal; border-bottom:1px solid #ffffff;', 1, '2012-10-24 17:32:07',1,'2012-10-24 17:32:07',NULL,NULL,NULL,NULL,NULL)$$

-- TRUNCATE TABLE slt_report_criteria$$
-- TRUNCATE TABLE slt_report_details$$
-- TRUNCATE TABLE slt_report_query_builder$$
TRUNCATE TABLE slt_report_schedules$$
TRUNCATE TABLE slt_report_schedules_history$$
TRUNCATE TABLE slt_report_schedules_queue$$
-- TRUNCATE TABLE slt_report_tables_alias$$
-- TRUNCATE TABLE slt_report_tables_fields_alias$$
-- TRUNCATE TABLE slt_report_tables_relation$$
-- TRUNCATE TABLE slt_report_template$$
-- TRUNCATE TABLE slt_report_themes$$
TRUNCATE TABLE slt_saved_queries$$
TRUNCATE TABLE slt_session_instructor_details$$
TRUNCATE TABLE slt_sf_enrollment$$
TRUNCATE TABLE slt_site_configuration$$

INSERT INTO `slt_site_configuration` (`code`, `name`, `enabled`, `value`, `remarks`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES ('ste_con_dep', 'Department', '1', NULL, NULL, '1', now(), '1', now(), NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_site_configuration` (`code`, `name`, `enabled`, `value`, `remarks`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES ('ste_con_ety', 'Employment Type', '1', NULL, NULL, '1', now(), '1', now(), NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_site_configuration` (`code`, `name`, `enabled`, `value`, `remarks`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES ('ste_con_jbr', 'Job Role', '1', NULL, NULL, '1', now(), '1', now(), NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_site_configuration` (`code`, `name`, `enabled`, `value`, `remarks`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES ('ste_con_lng', 'Language', '1', NULL, NULL, '1', now(), '1', now(), NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_site_configuration` (`code`, `name`, `enabled`, `value`, `remarks`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES ('ste_con_org', 'Organization', '1', NULL, NULL, '1', now(), '1', now(), NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_site_configuration` (`code`, `name`, `enabled`, `value`, `remarks`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES ('ste_con_rol', 'Role', '1', NULL, NULL, '1', now(), '1', now(), NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_site_configuration` (`code`, `name`, `enabled`, `value`, `remarks`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES ('ste_con_usr', 'User Type', '1', NULL, NULL, '1', now(), '1', now(), NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_site_configuration` (`code`, `name`, `enabled`, `value`, `remarks`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES ('ste_con_cnt', 'Country', '1', NULL, NULL, '1', now(), '1', now(), NULL, NULL, NULL, NULL, NULL)$$
INSERT INTO `slt_site_configuration` (`code`, `name`, `enabled`, `value`, `remarks`, `created_by`, `created_on`, `updated_by`, `updated_on`, `custom0`, `custom1`, `custom2`, `custom3`, `custom4`) VALUES ('ste_con_ste', 'State', '1', NULL, NULL, '1', now(), '1', now(), NULL, NULL, NULL, NULL, NULL)$$

TRUNCATE TABLE slt_site_notice$$
TRUNCATE TABLE slt_site_notice_dismiss$$
TRUNCATE TABLE slt_skill_set$$
TRUNCATE TABLE slt_sms_msg$$
-- TRUNCATE TABLE slt_state$$
TRUNCATE TABLE slt_survey$$
TRUNCATE TABLE slt_survey_groups$$
TRUNCATE TABLE slt_survey_groups_questions$$
TRUNCATE TABLE slt_survey_mapping$$
TRUNCATE TABLE slt_survey_questions$$
TRUNCATE TABLE slt_survey_results$$
TRUNCATE TABLE slt_survey_results_log$$
TRUNCATE TABLE slt_tag_entity$$
TRUNCATE TABLE slt_tagdefn$$
TRUNCATE TABLE slt_tax$$

insert  into `slt_tax`(`id`,`delivery_type`,`country`,`state`,`is_tax`,`product_code_id`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (1,'WBT','US','TX','Y','8312','admin','2010-08-12 23:14:54','admin','2010-08-12 23:14:54','','','','','')$$
insert  into `slt_tax`(`id`,`delivery_type`,`country`,`state`,`is_tax`,`product_code_id`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (2,'WBT','CA','AB','Y','01','admin','2010-08-12 23:14:54','admin','2010-08-12 23:14:54','','','','','')$$
insert  into `slt_tax`(`id`,`delivery_type`,`country`,`state`,`is_tax`,`product_code_id`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (3,'WBT','CA','BC','Y','01','admin','2010-08-12 23:14:54','admin','2010-08-12 23:14:54','','','','','')$$
insert  into `slt_tax`(`id`,`delivery_type`,`country`,`state`,`is_tax`,`product_code_id`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (4,'WBT','CA','MB','Y','01','admin','2010-08-12 23:14:54','admin','2010-08-12 23:14:54','','','','','')$$
insert  into `slt_tax`(`id`,`delivery_type`,`country`,`state`,`is_tax`,`product_code_id`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (5,'WBT','CA','NB','Y','01','admin','2010-08-12 23:14:54','admin','2010-08-12 23:14:54','','','','','')$$
insert  into `slt_tax`(`id`,`delivery_type`,`country`,`state`,`is_tax`,`product_code_id`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (6,'WBT','CA','NL','Y','01','admin','2010-08-12 23:14:54','admin','2010-08-12 23:14:54','','','','','')$$
insert  into `slt_tax`(`id`,`delivery_type`,`country`,`state`,`is_tax`,`product_code_id`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (7,'WBT','CA','NT','Y','01','admin','2010-08-12 23:14:54','admin','2010-08-12 23:14:54','','','','','')$$
insert  into `slt_tax`(`id`,`delivery_type`,`country`,`state`,`is_tax`,`product_code_id`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (8,'WBT','CA','NS','Y','01','admin','2010-08-12 23:14:54','admin','2010-08-12 23:14:54','','','','','')$$
insert  into `slt_tax`(`id`,`delivery_type`,`country`,`state`,`is_tax`,`product_code_id`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (9,'WBT','CA','NU','Y','01','admin','2010-08-12 23:14:54','admin','2010-08-12 23:14:54','','','','','')$$
insert  into `slt_tax`(`id`,`delivery_type`,`country`,`state`,`is_tax`,`product_code_id`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (10,'WBT','CA','ON','Y','01','admin','2010-08-12 23:14:54','admin','2010-08-12 23:14:54','','','','','')$$
insert  into `slt_tax`(`id`,`delivery_type`,`country`,`state`,`is_tax`,`product_code_id`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (11,'WBT','CA','PE','Y','01','admin','2010-08-12 23:14:54','admin','2010-08-12 23:14:54','','','','','')$$
insert  into `slt_tax`(`id`,`delivery_type`,`country`,`state`,`is_tax`,`product_code_id`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (12,'WBT','CA','QC','Y','01','admin','2010-08-12 23:14:54','admin','2010-08-12 23:14:54','','','','','')$$
insert  into `slt_tax`(`id`,`delivery_type`,`country`,`state`,`is_tax`,`product_code_id`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (13,'WBT','CA','SK','Y','01','admin','2010-08-12 23:14:54','admin','2010-08-12 23:14:54','','','','','')$$
insert  into `slt_tax`(`id`,`delivery_type`,`country`,`state`,`is_tax`,`product_code_id`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (14,'WBT','CA','YT','Y','01','admin','2010-08-12 23:14:54','admin','2010-08-12 23:14:54','','','','','')$$
insert  into `slt_tax`(`id`,`delivery_type`,`country`,`state`,`is_tax`,`product_code_id`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (15,'WBT','NG','','Y','01','admin','2010-08-12 23:14:54','admin','2010-08-12 23:14:54','','','','','')$$
insert  into `slt_tax`(`id`,`delivery_type`,`country`,`state`,`is_tax`,`product_code_id`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (16,'WBT','OTH','','N','02','admin','2010-08-12 23:14:54','admin','2010-08-12 23:14:54','','','','','')$$
insert  into `slt_tax`(`id`,`delivery_type`,`country`,`state`,`is_tax`,`product_code_id`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (17,'ILT','US','','N','02','admin','2010-08-12 23:14:54','admin','2010-08-12 23:14:54','','','','','')$$
insert  into `slt_tax`(`id`,`delivery_type`,`country`,`state`,`is_tax`,`product_code_id`,`created_by`,`created_on`,`updated_by`,`updated_on`,`custom0`,`custom1`,`custom2`,`custom3`,`custom4`) values (18,'ILT','OTH','','Y','01','admin','2010-08-12 23:14:54','admin','2010-08-12 23:14:54','','','','','')$$

TRUNCATE TABLE slt_tax_product_code_mapping$$
TRUNCATE TABLE slt_tax_settings$$
TRUNCATE TABLE slt_themeassociate$$
TRUNCATE TABLE slt_track_reminder_call$$
TRUNCATE TABLE slt_user_points$$
-- TRUNCATE TABLE slt_version$$
-- TRUNCATE TABLE slt_webservice$$
TRUNCATE TABLE slt_widget$$

-- LMS TABLES DATA RESET ENDS --$$