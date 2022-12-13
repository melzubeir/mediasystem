-- MySQL dump 10.13  Distrib 5.1.37, for debian-linux-gnu (i486)
--
-- Host: localhost    Database: socialhose
-- ------------------------------------------------------
-- Server version	5.1.37-1ubuntu5.5-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `agreement`
--

DROP TABLE IF EXISTS `agreement`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `agreement` (
  `id_agreement` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `id_customer` int(4) unsigned DEFAULT NULL,
  `name_agreement` text NOT NULL,
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`id_agreement`)
) ENGINE=MyISAM AUTO_INCREMENT=5710 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `article`
--

DROP TABLE IF EXISTS `article`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `article` (
  `id_article` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `headline_en` text NOT NULL,
  `headline_ar` text NOT NULL,
  `author` text NOT NULL,
  `publication_date` date DEFAULT NULL,
  `url` text NOT NULL,
  `issue_number` varchar(20) NOT NULL DEFAULT '',
  `location` varchar(40) NOT NULL DEFAULT '',
  `images_number` int(2) NOT NULL DEFAULT '1',
  `value` int(10) unsigned NOT NULL DEFAULT '0',
  `cost` int(10) unsigned NOT NULL DEFAULT '0',
  `published` tinyint(1) NOT NULL DEFAULT '0',
  `sent_morning` tinyint(1) DEFAULT '0',
  `sent_afternoon` tinyint(1) DEFAULT '0',
  `sent_evening` tinyint(1) DEFAULT '0',
  `id_summary` int(10) unsigned NOT NULL DEFAULT '0',
  `id_publication` int(4) unsigned NOT NULL DEFAULT '0',
  `id_article_type` int(4) unsigned NOT NULL DEFAULT '0',
  `id_article_impression` int(4) unsigned NOT NULL DEFAULT '0',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `published_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `section` text,
  `page_number` text NOT NULL,
  `published_date` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `circulation` int(10) unsigned DEFAULT NULL,
  `size` int(4) unsigned DEFAULT NULL,
  `locked_by` int(10) unsigned DEFAULT NULL,
  `unassigned` tinyint(1) DEFAULT NULL,
  `id_article_section` int(4) unsigned DEFAULT NULL,
  `ave` decimal(10,2) NOT NULL DEFAULT '0.00',
  `locked_date` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `id_zissor` int(10) unsigned DEFAULT NULL,
  `id_text_article` int(4) DEFAULT '0',
  `clip_type` tinyint(4) NOT NULL,
  `clip_duration` decimal(10,2) NOT NULL,
  `reject` tinyint(1) DEFAULT '0',
  `sent_express` tinyint(1) DEFAULT '0',
  `real_size` decimal(10,2) DEFAULT NULL,
  `percentage_size` decimal(10,2) DEFAULT NULL,
  `sent_express_continuous` tinyint(1) DEFAULT '0',
  `sent_morning_continuous` tinyint(1) DEFAULT '0',
  `towrite` tinyint(1) DEFAULT '0',
  `clipping_status` smallint(1) DEFAULT '0',
  `size_l` decimal(10,2) DEFAULT '0.00',
  `size_w` decimal(10,2) DEFAULT '0.00',
  `text_ocr` text NOT NULL,
  `sync` tinyint(1) DEFAULT NULL,
  `sent_max` tinyint(1) unsigned DEFAULT '0',
  PRIMARY KEY (`id_article`),
  KEY `id_summary` (`id_summary`),
  KEY `article_summary` (`id_summary`),
  KEY `created_by_idx` (`created_by`),
  KEY `created_idx` (`created`),
  KEY `publication_date_idx` (`publication_date`),
  KEY `id_publication_idx` (`id_publication`),
  KEY `published_by_idx` (`published_by`),
  KEY `published_date_idx` (`published_date`),
  KEY `sync_idx` (`sync`),
  KEY `clipping_status_idx` (`clipping_status`),
  KEY `locked_by_idx` (`locked_by`),
  KEY `towrite_idx` (`towrite`)
) ENGINE=MyISAM AUTO_INCREMENT=3575998 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `article_agreement`
--

DROP TABLE IF EXISTS `article_agreement`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `article_agreement` (
  `id_article_agreement` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `id_article` int(4) unsigned NOT NULL DEFAULT '0',
  `id_agreement` int(4) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_article_agreement`)
) ENGINE=MyISAM AUTO_INCREMENT=2330490 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `article_company`
--

DROP TABLE IF EXISTS `article_company`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `article_company` (
  `id_article_company` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_article` int(10) unsigned NOT NULL DEFAULT '0',
  `id_company` int(4) unsigned NOT NULL DEFAULT '0',
  `coordinates` varchar(120) DEFAULT '',
  PRIMARY KEY (`id_article_company`),
  KEY `id_article` (`id_article`,`id_company`),
  KEY `company_idx` (`id_company`)
) ENGINE=MyISAM AUTO_INCREMENT=6253456 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `article_customer`
--

DROP TABLE IF EXISTS `article_customer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `article_customer` (
  `id_article_customer` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `id_article` int(4) unsigned NOT NULL DEFAULT '0',
  `id_customer` int(4) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_article_customer`)
) ENGINE=MyISAM AUTO_INCREMENT=1561542 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `article_impression`
--

DROP TABLE IF EXISTS `article_impression`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `article_impression` (
  `id_article_impression` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `name_article_impression` varchar(26) NOT NULL DEFAULT '',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `name_article_impression_en` text NOT NULL,
  `name_article_impression_ar` text NOT NULL,
  PRIMARY KEY (`id_article_impression`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `article_industry`
--

DROP TABLE IF EXISTS `article_industry`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `article_industry` (
  `id_article_industry` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_article` int(10) unsigned NOT NULL DEFAULT '0',
  `id_industry` int(4) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_article_industry`),
  KEY `id_article` (`id_article`)
) ENGINE=MyISAM AUTO_INCREMENT=2296944 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `article_keywords`
--

DROP TABLE IF EXISTS `article_keywords`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `article_keywords` (
  `id_article_keywords` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_article` int(10) unsigned NOT NULL DEFAULT '0',
  `id_keywords` int(4) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_article_keywords`),
  KEY `id_article` (`id_article`)
) ENGINE=MyISAM AUTO_INCREMENT=883189 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `article_searchword`
--

DROP TABLE IF EXISTS `article_searchword`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `article_searchword` (
  `id_article_searchword` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_article` int(10) unsigned NOT NULL DEFAULT '0',
  `id_searchword` int(4) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_article_searchword`),
  KEY `id_article` (`id_article`)
) ENGINE=MyISAM AUTO_INCREMENT=264542 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `article_searchwords`
--

DROP TABLE IF EXISTS `article_searchwords`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `article_searchwords` (
  `id_article_searchwords` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_article` int(10) unsigned NOT NULL DEFAULT '0',
  `id_keywords` int(4) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_article_searchwords`),
  KEY `id_article` (`id_article`)
) ENGINE=MyISAM AUTO_INCREMENT=124 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `article_section`
--

DROP TABLE IF EXISTS `article_section`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `article_section` (
  `id_article_section` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `name_article_section_en` varchar(33) NOT NULL DEFAULT '',
  `name_article_section_ar` varchar(33) NOT NULL DEFAULT '',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`id_article_section`)
) ENGINE=MyISAM AUTO_INCREMENT=144 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `article_type`
--

DROP TABLE IF EXISTS `article_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `article_type` (
  `id_article_type` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `name_article_type` varchar(26) NOT NULL DEFAULT '',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `name_article_type_en` varchar(26) NOT NULL DEFAULT '',
  `name_article_type_ar` varchar(26) NOT NULL DEFAULT '',
  PRIMARY KEY (`id_article_type`)
) ENGINE=MyISAM AUTO_INCREMENT=16 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `category`
--

DROP TABLE IF EXISTS `category`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `category` (
  `id_category` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `name_category` varchar(26) NOT NULL DEFAULT '',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`id_category`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `clipping_coordinates`
--

DROP TABLE IF EXISTS `clipping_coordinates`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clipping_coordinates` (
  `id_clipping_table` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_article` int(10) unsigned NOT NULL,
  `id_article_section` int(4) unsigned NOT NULL,
  `page_number` text NOT NULL,
  `type` smallint(1) unsigned DEFAULT '0',
  `x` int(6) unsigned DEFAULT '0',
  `y` int(6) unsigned DEFAULT '0',
  `width` int(6) unsigned DEFAULT '0',
  `height` int(6) unsigned DEFAULT '0',
  PRIMARY KEY (`id_clipping_table`),
  KEY `article_idx` (`id_article`)
) ENGINE=MyISAM AUTO_INCREMENT=2857620 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `company`
--

DROP TABLE IF EXISTS `company`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `company` (
  `id_company` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `name_company` varchar(60) NOT NULL DEFAULT '',
  `address` varchar(60) NOT NULL DEFAULT '',
  `pobox` varchar(4) NOT NULL DEFAULT '',
  `city` varchar(27) NOT NULL DEFAULT '',
  `country` varchar(27) NOT NULL DEFAULT '',
  `phone` varchar(27) NOT NULL DEFAULT '',
  `fax` varchar(27) NOT NULL DEFAULT '',
  `email` varchar(27) NOT NULL DEFAULT '',
  `url` varchar(60) NOT NULL DEFAULT '',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `id_industry` int(4) unsigned DEFAULT NULL,
  `shortname_company` varchar(7) DEFAULT NULL,
  `towrite` int(1) unsigned DEFAULT NULL,
  `desc_company` varchar(255) DEFAULT '',
  `date_start` date DEFAULT NULL,
  `date_end` date DEFAULT NULL,
  `name_company_ar` varchar(60) DEFAULT '',
  PRIMARY KEY (`id_company`)
) ENGINE=MyISAM AUTO_INCREMENT=8577 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `country`
--

DROP TABLE IF EXISTS `country`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `country` (
  `id_country` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `name_country` varchar(26) NOT NULL DEFAULT '',
  `code_country` char(0) NOT NULL DEFAULT '',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `name_country_ar` varchar(26) DEFAULT NULL,
  `name_country_en` varchar(26) DEFAULT NULL,
  PRIMARY KEY (`id_country`)
) ENGINE=MyISAM AUTO_INCREMENT=59 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `custom_article`
--

DROP TABLE IF EXISTS `custom_article`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `custom_article` (
  `id_custom_article` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_article` int(10) unsigned NOT NULL DEFAULT '0',
  `id_users` int(4) unsigned NOT NULL DEFAULT '1',
  `brief` text,
  `note` text,
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `title` text,
  `size_cc` decimal(10,0) DEFAULT NULL,
  `ave` decimal(10,2) DEFAULT NULL,
  `size_page` decimal(10,4) DEFAULT NULL,
  `author` varchar(80) DEFAULT NULL,
  `pr_value` decimal(10,2) DEFAULT NULL,
  `spokesperson` varchar(80) DEFAULT NULL,
  PRIMARY KEY (`id_custom_article`,`id_article`,`id_users`),
  KEY `id_article_idx` (`id_article`)
) ENGINE=MyISAM AUTO_INCREMENT=102665 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `custom_prominence`
--

DROP TABLE IF EXISTS `custom_prominence`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `custom_prominence` (
  `id_custom_prominence` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `id_company` int(4) unsigned DEFAULT NULL,
  `id_article` int(4) unsigned DEFAULT NULL,
  `mention_score` int(2) unsigned NOT NULL DEFAULT '1',
  `visual_score` int(2) unsigned NOT NULL DEFAULT '0',
  `tone_score` int(2) NOT NULL DEFAULT '1',
  `size_cc` decimal(10,0) DEFAULT NULL,
  `ave` decimal(10,2) DEFAULT NULL,
  `size_page` decimal(10,4) DEFAULT NULL,
  `id_users` int(10) unsigned NOT NULL DEFAULT '0',
  `note` text,
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`id_custom_prominence`),
  KEY `article_idx` (`id_article`),
  KEY `company_idx` (`id_company`)
) ENGINE=MyISAM AUTO_INCREMENT=235631 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `customer`
--

DROP TABLE IF EXISTS `customer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `customer` (
  `id_customer` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `name_customer` text NOT NULL,
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`id_customer`)
) ENGINE=MyISAM AUTO_INCREMENT=173 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `emsOCR_status`
--

DROP TABLE IF EXISTS `emsOCR_status`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `emsOCR_status` (
  `id_emsOCR` int(4) unsigned NOT NULL,
  `latest_query_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `id_publication` int(4) unsigned DEFAULT NULL,
  `id_publication_issue` int(4) unsigned DEFAULT NULL,
  `latest_page_number` int(4) unsigned DEFAULT NULL,
  PRIMARY KEY (`id_emsOCR`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `frequency`
--

DROP TABLE IF EXISTS `frequency`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `frequency` (
  `id_frequency` int(2) unsigned NOT NULL AUTO_INCREMENT,
  `name_frequency` varchar(26) NOT NULL DEFAULT '',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`id_frequency`)
) ENGINE=MyISAM AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `industry`
--

DROP TABLE IF EXISTS `industry`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `industry` (
  `id_industry` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `name_industry` varchar(26) NOT NULL DEFAULT '',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`id_industry`)
) ENGINE=MyISAM AUTO_INCREMENT=266 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `issue_sections`
--

DROP TABLE IF EXISTS `issue_sections`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `issue_sections` (
  `id_issue_sections` int(6) unsigned NOT NULL AUTO_INCREMENT,
  `id_publication_issue` int(4) unsigned NOT NULL,
  `id_section` int(4) unsigned NOT NULL DEFAULT '1',
  `pages_reset` tinyint(1) DEFAULT '1',
  `page_start` int(2) unsigned NOT NULL DEFAULT '1',
  `page_end` int(2) unsigned NOT NULL DEFAULT '1',
  `pages_number` int(4) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_issue_sections`),
  KEY `id_publication_issue` (`id_publication_issue`),
  KEY `id_section` (`id_section`),
  KEY `publication_issue_idx` (`id_publication_issue`),
  KEY `section_idx` (`id_section`)
) ENGINE=MyISAM AUTO_INCREMENT=181870 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `issue_status`
--

DROP TABLE IF EXISTS `issue_status`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `issue_status` (
  `id_issue_status` smallint(1) unsigned NOT NULL AUTO_INCREMENT,
  `name_issue_status` varchar(32) NOT NULL DEFAULT '',
  PRIMARY KEY (`id_issue_status`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `keywords`
--

DROP TABLE IF EXISTS `keywords`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `keywords` (
  `id_keywords` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `name_keywords` text NOT NULL,
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `id_company` int(4) unsigned DEFAULT NULL,
  PRIMARY KEY (`id_keywords`)
) ENGINE=MyISAM AUTO_INCREMENT=9027 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `newsletter`
--

DROP TABLE IF EXISTS `newsletter`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `newsletter` (
  `id_newsletter` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `id_users` int(10) unsigned NOT NULL DEFAULT '0',
  `morning_newsletter` tinyint(1) DEFAULT '0',
  `afternoon_newsletter` tinyint(1) DEFAULT '0',
  `evening_newsletter` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`id_newsletter`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `page_text`
--

DROP TABLE IF EXISTS `page_text`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `page_text` (
  `id_page_text` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_section_pages` int(10) unsigned NOT NULL,
  `text` text CHARACTER SET utf8 NOT NULL,
  `id_publication` int(10) unsigned NOT NULL,
  `id_publication_type` int(4) unsigned NOT NULL,
  `id_country` int(4) unsigned NOT NULL,
  `publication_date` date DEFAULT NULL,
  PRIMARY KEY (`id_page_text`),
  KEY `idx_section_pages` (`id_section_pages`)
) ENGINE=MyISAM AUTO_INCREMENT=1956564 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `preclipping_coordinates`
--

DROP TABLE IF EXISTS `preclipping_coordinates`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `preclipping_coordinates` (
  `id_clipping_table` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_article` int(10) unsigned NOT NULL,
  `id_article_section` int(4) unsigned NOT NULL,
  `page_number` text NOT NULL,
  `type` smallint(1) unsigned DEFAULT '0',
  `x` int(6) unsigned DEFAULT '0',
  `y` int(6) unsigned DEFAULT '0',
  `width` int(6) unsigned DEFAULT '0',
  `height` int(6) unsigned DEFAULT '0',
  PRIMARY KEY (`id_clipping_table`)
) ENGINE=MyISAM AUTO_INCREMENT=104 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `prominence`
--

DROP TABLE IF EXISTS `prominence`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `prominence` (
  `id_prominence` int(8) unsigned NOT NULL AUTO_INCREMENT,
  `id_company` int(4) unsigned DEFAULT NULL,
  `id_article` int(4) unsigned DEFAULT NULL,
  `mention_score` int(2) unsigned NOT NULL DEFAULT '1',
  `visual_score` int(2) unsigned NOT NULL DEFAULT '0',
  `tone_score` int(2) NOT NULL DEFAULT '1',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `size_cc` decimal(10,0) DEFAULT NULL,
  `size_page` decimal(10,4) DEFAULT NULL,
  `ave` decimal(10,2) DEFAULT NULL,
  `note` text,
  PRIMARY KEY (`id_prominence`),
  KEY `company_idx` (`id_company`),
  KEY `article_idx` (`id_article`)
) ENGINE=MyISAM AUTO_INCREMENT=4593203 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `publication`
--

DROP TABLE IF EXISTS `publication`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `publication` (
  `id_publication` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `name_publication_en` varchar(80) NOT NULL DEFAULT '',
  `name_publication_ar` varchar(80) NOT NULL DEFAULT '',
  `country` int(4) unsigned NOT NULL DEFAULT '0',
  `circulation` int(10) unsigned NOT NULL DEFAULT '0',
  `language` varchar(20) NOT NULL DEFAULT '',
  `column_width` int(10) unsigned NOT NULL DEFAULT '0',
  `id_frequency` int(2) unsigned NOT NULL DEFAULT '0',
  `id_publication_type` int(4) unsigned NOT NULL DEFAULT '0',
  `id_publication_size` int(4) unsigned NOT NULL DEFAULT '0',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `url` text,
  `distribution` int(4) unsigned DEFAULT NULL,
  `adrate_bw` decimal(10,2) DEFAULT NULL,
  `adrate_color` decimal(10,2) DEFAULT NULL,
  `id_publication_genre` int(4) unsigned DEFAULT NULL,
  `id_publication_target` int(4) unsigned DEFAULT NULL,
  `telephone` varchar(80) NOT NULL DEFAULT '',
  `fax` varchar(80) NOT NULL DEFAULT '',
  `email` varchar(80) NOT NULL DEFAULT '',
  `skip_ocr` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`id_publication`)
) ENGINE=MyISAM AUTO_INCREMENT=6899 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `publication_genre`
--

DROP TABLE IF EXISTS `publication_genre`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `publication_genre` (
  `id_publication_genre` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `name_publication_genre` varchar(26) NOT NULL DEFAULT '',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`id_publication_genre`)
) ENGINE=MyISAM AUTO_INCREMENT=49 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `publication_issue`
--

DROP TABLE IF EXISTS `publication_issue`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `publication_issue` (
  `id_publication_issue` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_publication` int(4) unsigned NOT NULL DEFAULT '0',
  `id_issue` int(10) unsigned NOT NULL DEFAULT '0',
  `publication_status` smallint(1) unsigned NOT NULL DEFAULT '1',
  `issue_date` date DEFAULT NULL,
  `pages_number` smallint(1) NOT NULL DEFAULT '0',
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `done_by` int(10) unsigned NOT NULL DEFAULT '0',
  `done_time` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id_publication_issue`),
  KEY `publication_idx` (`id_publication`)
) ENGINE=MyISAM AUTO_INCREMENT=105504 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `publication_rate`
--

DROP TABLE IF EXISTS `publication_rate`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `publication_rate` (
  `id_publication_rate` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `name_publication_rate` varchar(3) NOT NULL DEFAULT '',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`id_publication_rate`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `publication_size`
--

DROP TABLE IF EXISTS `publication_size`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `publication_size` (
  `id_publication_size` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `name_publication_size` varchar(26) NOT NULL DEFAULT '',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`id_publication_size`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `publication_target`
--

DROP TABLE IF EXISTS `publication_target`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `publication_target` (
  `id_publication_target` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `name_publication_target` varchar(26) NOT NULL DEFAULT '',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`id_publication_target`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `publication_type`
--

DROP TABLE IF EXISTS `publication_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `publication_type` (
  `id_publication_type` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `name_publication_type` varchar(26) NOT NULL DEFAULT '',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `name_publication_type_ar` varchar(26) NOT NULL DEFAULT '',
  `name_publication_type_en` varchar(26) NOT NULL DEFAULT '',
  PRIMARY KEY (`id_publication_type`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `publisher`
--

DROP TABLE IF EXISTS `publisher`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `publisher` (
  `id_publisher` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `name_publisher_en` varchar(33) NOT NULL DEFAULT '',
  `name_publisher_ar` varchar(33) NOT NULL DEFAULT '',
  `id_country` int(4) unsigned NOT NULL DEFAULT '0',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`id_publisher`)
) ENGINE=MyISAM AUTO_INCREMENT=11 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `report`
--

DROP TABLE IF EXISTS `report`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `report` (
  `id_report` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name_en` text NOT NULL,
  `name_ar` text NOT NULL,
  `id_client` int(10) unsigned NOT NULL DEFAULT '0',
  `id_company` int(10) unsigned NOT NULL DEFAULT '0',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`id_report`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `role`
--

DROP TABLE IF EXISTS `role`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `role` (
  `id_role` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `name_role` varchar(26) NOT NULL DEFAULT '',
  `type_role` varchar(26) NOT NULL DEFAULT 'client',
  `create_article` tinyint(1) DEFAULT '0',
  `read_article` tinyint(1) DEFAULT '1',
  `modify_article` tinyint(1) DEFAULT '0',
  `delete_article` tinyint(1) DEFAULT '0',
  `create_article_type` tinyint(1) DEFAULT '0',
  `read_article_type` tinyint(1) DEFAULT '1',
  `modify_article_type` tinyint(1) DEFAULT '0',
  `delete_article_type` tinyint(1) DEFAULT '0',
  `create_article_impression` tinyint(1) DEFAULT '0',
  `read_article_impression` tinyint(1) DEFAULT '1',
  `modify_article_impression` tinyint(1) DEFAULT '0',
  `delete_article_impression` tinyint(1) DEFAULT '0',
  `create_summary` tinyint(1) DEFAULT '0',
  `read_summary` tinyint(1) DEFAULT '1',
  `modify_summary` tinyint(1) DEFAULT '0',
  `delete_summary` tinyint(1) DEFAULT '0',
  `create_users` tinyint(1) DEFAULT '0',
  `read_users` tinyint(1) DEFAULT '1',
  `modify_users` tinyint(1) DEFAULT '0',
  `delete_users` tinyint(1) DEFAULT '0',
  `create_company` tinyint(1) DEFAULT '0',
  `read_company` tinyint(1) DEFAULT '1',
  `modify_company` tinyint(1) DEFAULT '0',
  `delete_company` tinyint(1) DEFAULT '0',
  `create_industry` tinyint(1) DEFAULT '0',
  `read_industry` tinyint(1) DEFAULT '1',
  `modify_industry` tinyint(1) DEFAULT '0',
  `delete_industry` tinyint(1) DEFAULT '0',
  `create_keywords` tinyint(1) DEFAULT '0',
  `read_keywords` tinyint(1) DEFAULT '1',
  `modify_keywords` tinyint(1) DEFAULT '0',
  `delete_keywords` tinyint(1) DEFAULT '0',
  `create_publication` tinyint(1) DEFAULT '0',
  `read_publication` tinyint(1) DEFAULT '1',
  `modify_publication` tinyint(1) DEFAULT '0',
  `delete_publication` tinyint(1) DEFAULT '0',
  `create_publication_rate` tinyint(1) DEFAULT '0',
  `read_publication_rate` tinyint(1) DEFAULT '1',
  `modify_publication_rate` tinyint(1) DEFAULT '0',
  `delete_publication_rate` tinyint(1) DEFAULT '0',
  `create_publication_size` tinyint(1) DEFAULT '0',
  `read_publication_size` tinyint(1) DEFAULT '1',
  `modify_publication_size` tinyint(1) DEFAULT '0',
  `delete_publication_size` tinyint(1) DEFAULT '0',
  `create_publication_type` tinyint(1) DEFAULT '0',
  `read_publication_type` tinyint(1) DEFAULT '1',
  `modify_publication_type` tinyint(1) DEFAULT '0',
  `delete_publication_type` tinyint(1) DEFAULT '0',
  `publish` tinyint(1) DEFAULT '0',
  `unpublish` tinyint(1) DEFAULT '0',
  `newsletter` tinyint(1) DEFAULT '0',
  `create_country` tinyint(1) DEFAULT NULL,
  `read_country` tinyint(1) DEFAULT NULL,
  `modify_country` tinyint(1) DEFAULT NULL,
  `delete_country` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`id_role`)
) ENGINE=MyISAM AUTO_INCREMENT=100 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `searchword`
--

DROP TABLE IF EXISTS `searchword`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `searchword` (
  `id_searchword` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `name_searchword` text NOT NULL,
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `id_company` int(4) unsigned DEFAULT NULL,
  `name_searchword_ar` varchar(60) DEFAULT '',
  `case_sensitive` smallint(1) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_searchword`),
  KEY `company_idx` (`id_company`),
  KEY `company_sw_idx` (`id_company`,`id_searchword`)
) ENGINE=MyISAM AUTO_INCREMENT=48467 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `section_pages`
--

DROP TABLE IF EXISTS `section_pages`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `section_pages` (
  `section_pages` int(11) NOT NULL AUTO_INCREMENT,
  `id_issue_sections` int(6) unsigned DEFAULT NULL,
  `page_name` varchar(10) NOT NULL,
  `locked_by` int(10) unsigned DEFAULT '0',
  `status` int(11) DEFAULT '0',
  PRIMARY KEY (`section_pages`),
  KEY `idx_issue_sections` (`id_issue_sections`),
  KEY `locked_by_idx` (`locked_by`),
  KEY `status_idx` (`status`)
) ENGINE=MyISAM AUTO_INCREMENT=6667914 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `summary`
--

DROP TABLE IF EXISTS `summary`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `summary` (
  `id_summary` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `en` text NOT NULL,
  `ar` text NOT NULL,
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `headline` text,
  PRIMARY KEY (`id_summary`),
  KEY `summary_idx` (`id_summary`),
  KEY `created_idx` (`created`)
) ENGINE=MyISAM AUTO_INCREMENT=2408603 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `system_configuration`
--

DROP TABLE IF EXISTS `system_configuration`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `system_configuration` (
  `clippings_path` varchar(255) NOT NULL DEFAULT '',
  `issues_path` varchar(255) NOT NULL DEFAULT '',
  `skip_ocr` smallint(1) unsigned DEFAULT '0',
  `clippings_windows_drive_letter` varchar(3) NOT NULL DEFAULT '',
  `issues_windows_drive_letter` varchar(3) NOT NULL DEFAULT '',
  `top_margine_first_page` decimal(2,1) NOT NULL DEFAULT '6.0',
  `top_margine_other_pages` decimal(2,1) NOT NULL DEFAULT '3.0',
  `bottom_margine` decimal(2,1) NOT NULL DEFAULT '1.0',
  `left_margine` decimal(2,1) NOT NULL DEFAULT '1.0',
  `right_margine` decimal(2,1) NOT NULL DEFAULT '1.0',
  `wiki_url` varchar(255) NOT NULL DEFAULT ''
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `text_article`
--

DROP TABLE IF EXISTS `text_article`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `text_article` (
  `id_text_article` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `text_article` text NOT NULL,
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`id_text_article`)
) ENGINE=MyISAM AUTO_INCREMENT=246609 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_page_track`
--

DROP TABLE IF EXISTS `user_page_track`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_page_track` (
  `id_user_page_track` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_users` int(4) NOT NULL DEFAULT '0',
  `id_section_pages` int(10) NOT NULL DEFAULT '0',
  `last_modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `first_access` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `last_access` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `issue_created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `id_publication` int(4) unsigned NOT NULL DEFAULT '0',
  `id_publication_type` int(4) unsigned NOT NULL DEFAULT '0',
  `publication_language` varchar(30) NOT NULL DEFAULT '',
  `publication_market` int(4) unsigned NOT NULL DEFAULT '0',
  `issue_date` date DEFAULT NULL,
  `seconds` int(4) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_user_page_track`)
) ENGINE=MyISAM AUTO_INCREMENT=268 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
  `id_users` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(60) DEFAULT NULL,
  `id_role` int(4) unsigned NOT NULL DEFAULT '0',
  `expiry` date DEFAULT NULL,
  `fname` varchar(60) DEFAULT NULL,
  `lname` varchar(60) DEFAULT NULL,
  `email` varchar(60) DEFAULT NULL,
  `password` varchar(27) NOT NULL DEFAULT '',
  `phone` varchar(27) NOT NULL DEFAULT '',
  `mobile` varchar(27) NOT NULL DEFAULT '',
  `fax` varchar(27) NOT NULL DEFAULT '',
  `address` varchar(60) DEFAULT NULL,
  `pobox` varchar(4) NOT NULL DEFAULT '',
  `city` varchar(27) NOT NULL DEFAULT '',
  `country` varchar(60) DEFAULT NULL,
  `id_company` int(4) unsigned NOT NULL DEFAULT '0',
  `created_by` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `created` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `id_mar` text,
  `package` int(1) unsigned NOT NULL DEFAULT '0',
  `mar_style` text NOT NULL,
  `id_fav_company` int(4) unsigned DEFAULT NULL,
  `start_date` date DEFAULT '2008-01-01',
  `layout_tbl_bg_color` char(6) DEFAULT 'ffffff',
  `layout_tbl_border_color` char(6) DEFAULT '000000',
  `layout_tbl_font_color` char(6) DEFAULT '000000',
  `layout_font_color` char(6) DEFAULT '000000',
  `layout_logo_align` int(1) DEFAULT '1',
  `layout_inc_title` tinyint(1) DEFAULT '1',
  `layout_inc_brief` tinyint(1) DEFAULT '1',
  `layout_inc_ave` tinyint(1) DEFAULT '1',
  `ar_frequency` int(1) DEFAULT '1',
  `layout_inc_pr_value` tinyint(1) DEFAULT NULL,
  `tools` tinyint(1) DEFAULT NULL,
  `layout_inc_fp_ave` tinyint(1) unsigned DEFAULT '0',
  PRIMARY KEY (`id_users`)
) ENGINE=MyISAM AUTO_INCREMENT=4631 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `users_company`
--

DROP TABLE IF EXISTS `users_company`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users_company` (
  `id_users_company` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_users` int(10) unsigned NOT NULL DEFAULT '0',
  `id_company` int(4) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_users_company`)
) ENGINE=MyISAM AUTO_INCREMENT=375389 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `users_country`
--

DROP TABLE IF EXISTS `users_country`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users_country` (
  `id_users_country` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_users` int(10) unsigned NOT NULL DEFAULT '0',
  `id_country` int(4) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_users_country`)
) ENGINE=MyISAM AUTO_INCREMENT=159087 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `users_industry`
--

DROP TABLE IF EXISTS `users_industry`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users_industry` (
  `id_users_industry` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_users` int(10) unsigned NOT NULL DEFAULT '0',
  `id_industry` int(4) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_users_industry`)
) ENGINE=MyISAM AUTO_INCREMENT=21461 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `users_keywords`
--

DROP TABLE IF EXISTS `users_keywords`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users_keywords` (
  `id_users_keywords` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `id_users` int(10) unsigned NOT NULL DEFAULT '0',
  `id_keywords` int(4) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id_users_keywords`)
) ENGINE=MyISAM AUTO_INCREMENT=175235 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2011-03-30  9:31:35
