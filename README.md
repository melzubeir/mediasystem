# Socialhose Media System

A comprehensive Qt/C++ desktop application for print media clipping, OCR processing, and digital content management. The system provides professional tools for scanning, processing, and organizing print media articles with advanced OCR capabilities and modern API integration.

## Project Overview

The Socialhose Media System is a full-featured print news clipping platform designed for media monitoring organizations, news agencies, and content management teams. It combines powerful desktop applications with modern REST API integration for seamless workflow management.

### Key Components

- **ClippingStation**: Main GUI application for scanning, clipping, and managing print media articles
- **emsOCR**: High-performance OCR engine with crash recovery and auto-restart capabilities  
- **Socialhose API Integration**: Modern REST API backend for campaign management and data synchronization

### Target Platforms
- Windows (Primary for OCR Server)
- Linux (Ubuntu/Debian - Primary Clipping Station)
- macOS (Limited support)

## Architecture

### System Architecture
```
┌─────────────────┐    ┌─────────────────┐   
│  ClippingStation│    │    emsOCR       │    
│  (Main GUI)     │◄──►│  (OCR Engine)   │   
│  Qt/C++ Desktop │    │  Process-based  │   
└─────────────────┘    └─────────────────┘   
         │                       │           
         ▼                       ▼           
┌─────────────────────────────────────────────────────────────────┐
│                    Data Layer                                   │
├─────────────────┬─────────────────┬─────────────────────────────┤
│   MySQL DB      │  Socialhose API │    File System              │
│   (Legacy)      │  (New Backend)  │    (Images/Cache)         │
└─────────────────┴─────────────────┴─────────────────────────────┘
```

### Technology Stack
- **Frontend**: Qt 6 (C++) with Qt Widgets
- **Backend**: MySQL Database (Legacy) + Socialhose REST API (New)
- **OCR**: IRIS iDRS OCR engine with advanced Arabic and English text recognition
- **Build System**: qmake/Visual Studio
- **Platforms**: Cross-platform desktop application

### Database Architecture
The system uses MySQL with a comprehensive schema supporting:
- **Articles**: Main article storage with OCR text, metadata, and clipping information
- **Publications**: Publication management and issue tracking
- **Users**: Authentication, permissions, and user profiles
- **Coordinates**: Precise clipping regions and OCR word positioning
- **Tags**: Article categorization and keyword management
- **Campaigns**: Socialhose API integration for modern workflow management

## Features

### Core Functionality
- **Print Media Scanning**: High-resolution scanning with automatic page detection
- **Smart Clipping**: Intuitive drag-and-drop interface for article extraction
- **Advanced OCR**: Multi-engine OCR with Arabic and English text recognition
- **Intelligent Tagging**: Automatic keyword extraction and manual categorization
- **Analytics**: Article metrics, circulation data, and impression tracking
- **Multi-user Support**: Role-based access control with user authentication

### Advanced Features
- **Intelligent Caching**: Memory-efficient image caching with thumbnail generation
- **Network Image Support**: Remote image loading with local caching
- **Real-time Processing**: Background OCR processing with progress monitoring
- **API Integration**: Modern REST API for cloud synchronization
- **Crash Recovery**: Automatic OCR engine restart with Windows crash dump generation
- **Precision Tools**: Zoom, rotation, and fine-tuning controls for accurate clipping

### Socialhose API Integration (New)
- **Campaign Management**: Create and manage monitoring campaigns
- **Mention Tracking**: Track articles as social media mentions
- **Keyword Monitoring**: Real-time keyword and brand monitoring
- **Organization Support**: Multi-tenant organization-based access
- **JWT Authentication**: Secure token-based authentication
- **Hybrid Data Mode**: Seamless switching between API and local database

## 🛠️ Build Instructions

### Prerequisites
- **Qt 6.x** (Successfully migrated from Qt 4)
- **MySQL Server** 5.7+ or MariaDB
- **C++ Compiler** (MSVC, GCC, or Clang)
- **CMake** 3.16+ or qmake

### Quick Build (Qt 6)
```bash
# Clone the repository
git clone <repository-url>
cd mediasystem

# Build ClippingStation (Main Application)
cd ClippingStation
qmake ClippingStation.pro
make

# Build emsOCR (OCR Engine)
cd ../emsOCR
qmake emsOCR.pro
make

# Build OurOCREngine (Alternative OCR)
cd ../OurOCREngine
qmake OurOCREngine.pro
make
```

### Windows Build (Visual Studio)
```bash
# Open solution files in Visual Studio
ClippingStation/ClippingStation.sln
emsOCR/emsOCR.sln
OurOCREngine/OurOCREngine.sln

# Build using Visual Studio IDE
```

### Qt Creator Build
```bash
# Open project files in Qt Creator
ClippingStation/ClippingStation.pro
emsOCR/emsOCR.pro
OurOCREngine/OurOCREngine.pro

# Build and run from Qt Creator IDE
```

### Clean Build
```bash
# Clean previous builds
make clean

# Rebuild from scratch
qmake
make
```

## Database Setup

### MySQL Database Configuration
```sql
-- Create database
CREATE DATABASE socialhose CHARACTER SET utf8 COLLATE utf8_unicode_ci;

-- Import schema
mysql -u root -p socialhose < db_schema.sql

-- Configure connection in config.ini
[master database]
server=localhost
port=3306
database=socialhose
uid=root
pwd=your_password
```

### Configuration File (config.ini)
```ini
[master database]
server=localhost
port=3306
database=socialhose
uid=root
pwd=your_password

[api]
base_url=http://localhost:8001
timeout=30000
data_source=hybrid
enabled=true
remember_login=false

[shortcuts]
Clip=Ctrl+S
Preclip=Ctrl+D
AddPage=Ctrl+A
NextPage=Ctrl+E
PreviousPage=Ctrl+W

[debug mode]
debug=on
```

## 🔧 Development Guidelines

### Code Architecture
- **MVC Pattern**: Models, Views, and Controllers clearly separated
- **Repository Pattern**: Data access abstraction with API/Database dual mode
- **Signal-Slot Mechanism**: Qt's event-driven communication pattern
- **Process Isolation**: OCR engines run as separate processes for stability

### Key Classes and Components
- **ClippingStation**: Main application window and coordinator
- **FullPageView**: High-resolution image viewer with clipping tools
- **DrawerView**: Article composition and layout management
- **OcrThread**: Background OCR processing with progress tracking
- **ImageCache**: Memory-efficient image caching system
- **SocialhoseApiClient**: Modern REST API client with JWT authentication

### API Integration Architecture
```cpp
// Example: Repository Pattern Usage
CampaignRepository* repo = new CampaignRepository();
connect(repo, &CampaignRepository::campaignCreated, this, &MainWindow::onCampaignCreated);

// Fetch campaigns from API or database
QList<SocialhoseDTO::Campaign> campaigns = repo->getAll();

// Create new campaign
SocialhoseDTO::Campaign newCampaign;
newCampaign.name = "New Publication";
newCampaign = repo->create(newCampaign);
```

### Error Handling
- **Network Failures**: Automatic retry with exponential backoff
- **Authentication**: Token refresh and re-login on expiration
- **OCR Crashes**: Automatic process restart with crash dump generation
- **Database Errors**: Graceful degradation with user notifications

### Performance Optimization
- **Image Caching**: Multi-level caching (memory, disk, network)
- **Lazy Loading**: On-demand image and data loading
- **Background Processing**: Non-blocking OCR and network operations
- **Memory Management**: Qt's parent-child object hierarchy for automatic cleanup

## 🧪 Testing

### Manual Testing Checklist
- [ ] Image loading and display performance
- [ ] OCR accuracy for Arabic and English text
- [ ] Clipping precision and coordinate accuracy
- [ ] Multi-user authentication and permissions
- [ ] API integration with Socialhose backend
- [ ] Crash recovery and error handling
- [ ] Cross-platform compatibility

### Integration Testing
```bash
# Test API connectivity
cd ClippingStation
cp test_api_client.cpp main.cpp
qmake && make
./ClippingStation

# Verify database connectivity
mysql -u root -p socialhose -e "SELECT COUNT(*) FROM articles;"
```

## Project Structure

```
mediasystem/
├── ClippingStation/          # Main GUI application
│   ├── api/                  # Socialhose API integration
│   │   ├── dto/              # Data Transfer Objects
│   │   └── repositories/     # Repository pattern implementation
│   ├── clippingstation.*     # Main application files
│   ├── fullpageview.*        # Image viewer component
│   ├── drawerview.*          # Article composition tool
│   └── config.ini            # Application configuration
├── emsOCR/                   # Primary OCR engine
│   ├── main.cpp              # Process entry point
│   └── emsocrdialog.*        # OCR processing dialog
├── db_schema.sql             # Complete database schema
└── README.md                 # This file
```

## API Documentation

### Socialhose API Integration
The system now supports modern REST API integration with the Socialhose service:

**Authentication**: JWT-based with automatic token refresh
**Endpoints**: Full CRUD operations for campaigns, mentions, and keywords
**Data Sources**: Hybrid mode supports both API and local database
**Offline Support**: Graceful fallback to local database when offline

For detailed API documentation, see:
- `INTEGRATION-PLAN.md` - Complete integration roadmap
- `PHASE1-COMPLETE.md` - Completed foundation phase
- `ClippingStation/api/` - Implementation details

## Troubleshooting

### Common Issues
1. **Qt 6 Migration**: Project successfully migrated, use Qt 6.x only
2. **Database Connection**: Check MySQL service and config.ini settings
3. **OCR Engine Crashes**: Automatic restart enabled, check MiniDump.dmp for details
4. **API Connection**: Verify base_url in config.ini and network connectivity

### Debug Mode
Enable debug logging in config.ini:
```ini
[debug mode]
debug=on
```

### Support
- Check existing documentation: `CLAUDE.md`, `INTEGRATION-PLAN.md`
- Review database schema: `db_schema.sql`
- Examine configuration: `ClippingStation/config.ini`

## License
This project is licensed under the GNU General Public License v3.0 (GPLv3).  
See the LICENSE file for full license details.

---

**Last Updated**: December 2024  
**Qt Version**: 6.x (Successfully migrated from Qt 4)  
**API Integration**: In Progress
**Build Status**: All components building successfully
