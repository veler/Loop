USE [master]
GO
/****** Object:  Database [tweekbrosdatabase]    Script Date: 6/11/2014 8:53:22 PM ******/
CREATE DATABASE [tweekbrosdatabase]
 CONTAINMENT = NONE
 ON  PRIMARY 
( NAME = N'ProjetFinAnnee', FILENAME = N'C:\Users\Etienne\ProjetFinAnnee.mdf' , SIZE = 44032KB , MAXSIZE = UNLIMITED, FILEGROWTH = 1024KB )
 LOG ON 
( NAME = N'ProjetFinAnnee_log', FILENAME = N'C:\Users\Etienne\ProjetFinAnnee_log.ldf' , SIZE = 15040KB , MAXSIZE = 2048GB , FILEGROWTH = 10%)
GO
ALTER DATABASE [tweekbrosdatabase] SET COMPATIBILITY_LEVEL = 110
GO
IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [tweekbrosdatabase].[dbo].[sp_fulltext_database] @action = 'enable'
end
GO
ALTER DATABASE [tweekbrosdatabase] SET ANSI_NULL_DEFAULT OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET ANSI_NULLS OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET ANSI_PADDING OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET ANSI_WARNINGS OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET ARITHABORT OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET AUTO_CLOSE OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET AUTO_CREATE_STATISTICS ON 
GO
ALTER DATABASE [tweekbrosdatabase] SET AUTO_SHRINK OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET AUTO_UPDATE_STATISTICS ON 
GO
ALTER DATABASE [tweekbrosdatabase] SET CURSOR_CLOSE_ON_COMMIT OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET CURSOR_DEFAULT  GLOBAL 
GO
ALTER DATABASE [tweekbrosdatabase] SET CONCAT_NULL_YIELDS_NULL OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET NUMERIC_ROUNDABORT OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET QUOTED_IDENTIFIER OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET RECURSIVE_TRIGGERS OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET  DISABLE_BROKER 
GO
ALTER DATABASE [tweekbrosdatabase] SET AUTO_UPDATE_STATISTICS_ASYNC OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET DATE_CORRELATION_OPTIMIZATION OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET TRUSTWORTHY OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET ALLOW_SNAPSHOT_ISOLATION OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET PARAMETERIZATION SIMPLE 
GO
ALTER DATABASE [tweekbrosdatabase] SET READ_COMMITTED_SNAPSHOT OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET HONOR_BROKER_PRIORITY OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET RECOVERY FULL 
GO
ALTER DATABASE [tweekbrosdatabase] SET  MULTI_USER 
GO
ALTER DATABASE [tweekbrosdatabase] SET PAGE_VERIFY CHECKSUM  
GO
ALTER DATABASE [tweekbrosdatabase] SET DB_CHAINING OFF 
GO
ALTER DATABASE [tweekbrosdatabase] SET FILESTREAM( NON_TRANSACTED_ACCESS = OFF ) 
GO
ALTER DATABASE [tweekbrosdatabase] SET TARGET_RECOVERY_TIME = 0 SECONDS 
GO
USE [tweekbrosdatabase]
GO
/****** Object:  User [RobotApp]    Script Date: 6/11/2014 8:53:22 PM ******/
CREATE USER [RobotApp] FOR LOGIN [RobotApp] WITH DEFAULT_SCHEMA=[dbo]
GO
ALTER ROLE [db_owner] ADD MEMBER [RobotApp]
GO
/****** Object:  Table [dbo].[Balises]    Script Date: 6/11/2014 8:53:22 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Balises](
	[PK_Balises] [int] IDENTITY(1,1) NOT NULL,
	[FK_PageInternet] [int] NOT NULL,
	[type] [nvarchar](20) NOT NULL,
	[content] [nvarchar](max) NOT NULL,
 CONSTRAINT [PK_Balises] PRIMARY KEY CLUSTERED 
(
	[PK_Balises] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
/****** Object:  Table [dbo].[Domains]    Script Date: 6/11/2014 8:53:22 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[Domains](
	[PK_Domain] [int] IDENTITY(1,1) NOT NULL,
	[Domain] [varchar](max) NOT NULL,
 CONSTRAINT [PK_Domains] PRIMARY KEY CLUSTERED 
(
	[PK_Domain] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[PageInternet]    Script Date: 6/11/2014 8:53:22 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PageInternet](
	[PK_PageInternet] [int] IDENTITY(1,1) NOT NULL,
	[URL] [nvarchar](max) NOT NULL,
	[Title] [nvarchar](max) NULL,
	[Description] [nvarchar](max) NULL,
	[Keywords] [nvarchar](max) NULL,
	[ViewCount] [int] NOT NULL,
	[FavoriteCount] [int] NOT NULL,
	[ShareCount] [int] NOT NULL,
	[LikeCount] [int] NOT NULL,
	[Date] [int] NOT NULL,
 CONSTRAINT [PK_PageInternet] PRIMARY KEY CLUSTERED 
(
	[PK_PageInternet] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
/****** Object:  Table [dbo].[Photo]    Script Date: 6/11/2014 8:53:22 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Photo](
	[PK_Photo] [int] IDENTITY(1,1) NOT NULL,
	[URL] [nvarchar](max) NOT NULL,
	[Title] [nvarchar](max) NULL,
	[Description] [nvarchar](max) NULL,
	[Height] [int] NULL,
	[Width] [int] NULL,
	[Resolution] [nvarchar](max) NULL,
	[Color] [nvarchar](max) NULL,
	[ViewCount] [int] NOT NULL,
	[ShareCount] [int] NOT NULL,
	[LikeCount] [int] NOT NULL,
 CONSTRAINT [PK_Table_1] PRIMARY KEY CLUSTERED 
(
	[PK_Photo] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
ALTER TABLE [dbo].[Balises]  WITH CHECK ADD  CONSTRAINT [FK_Table_PageInternet] FOREIGN KEY([FK_PageInternet])
REFERENCES [dbo].[PageInternet] ([PK_PageInternet])
GO
ALTER TABLE [dbo].[Balises] CHECK CONSTRAINT [FK_Table_PageInternet]
GO
USE [master]
GO
ALTER DATABASE [tweekbrosdatabase] SET  READ_WRITE 
GO
