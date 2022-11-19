-- Create New Database For MQTT2MYSQL Application

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema mydb
-- -----------------------------------------------------
-- -----------------------------------------------------
-- Schema mqtt
-- -----------------------------------------------------
DROP SCHEMA IF EXISTS `mqtt` ;

-- -----------------------------------------------------
-- Schema mqtt
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `mqtt` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci ;
USE `mqtt` ;

-- -----------------------------------------------------
-- Table `mqtt`.`topics`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `mqtt`.`topics` ;

CREATE TABLE IF NOT EXISTS `mqtt`.`topics` (
  `topic_id` BIGINT(20) NOT NULL AUTO_INCREMENT,
  `topic` VARCHAR(200) NOT NULL,
  PRIMARY KEY (`topic_id`))
ENGINE = InnoDB
COMMENT = 'MQTT Topics';

-- -----------------------------------------------------
-- Table `mqtt`.`data`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `mqtt`.`data` ;

CREATE TABLE IF NOT EXISTS `mqtt`.`data` (
  `value_timestamp` TIMESTAMP(3) NOT NULL,
  `value` VARCHAR(3000) NOT NULL,
  `topic_id` BIGINT NOT NULL,
  PRIMARY KEY (`value_timestamp`, `topic_id`),
  INDEX `fk_data_topics_idx` (`topic_id` ASC) VISIBLE,
  CONSTRAINT `fk_data_topics`
    FOREIGN KEY (`topic_id`)
    REFERENCES `mqtt`.`topics` (`topic_id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
COMMENT = 'MQTT Topic Values';


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;

