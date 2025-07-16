# Changelog

All notable changes to this project will be documented in this file.

## [1.1.10] - 2025-01-27

### Added
- **Frame Success/Failure Tracking**: New communication monitoring system
  - `getFrameSuccess()` - Returns total number of successful frame exchanges
  - `getFrameFailure()` - Returns total number of failed frame exchanges  
  - `getLastSuccessTime()` - Returns timestamp of last successful communication
- Real-time communication health monitoring for better debugging and reliability
- Automatic tracking integrated into existing onSuccess/onError handler infrastructure

### Changed
- Enhanced `ApplianceBase` class with communication statistics
- Improved error handling and debugging capabilities

### Technical Details
- Added `m_frameSuccess`, `m_frameFailure`, and `m_lastSuccessTime` member variables to `ApplianceBase`
- Success tracking increments when frames are successfully processed by AC units
- Failure tracking increments when frames timeout or are rejected
- Timestamp tracking helps monitor connection health and detect communication issues
- Fully backward compatible - existing code continues to work unchanged

### Tested
- Verified with real Midea AC hardware
- Confirmed accurate tracking of successful and failed communications
- Validated timestamp accuracy for connection health monitoring

## [1.2.0] - 2025-07-16

### Added
- **Frame Success/Failure Tracking**: New communication monitoring system
  - `getFrameSuccess()` - Returns total number of successful frame exchanges
  - `getFrameFailure()` - Returns total number of failed frame exchanges  
  - `getLastSuccessTime()` - Returns timestamp of last successful communication
- Real-time communication health monitoring for better debugging and reliability
- Automatic tracking integrated into existing onSuccess/onError handler infrastructure

### Changed
- Enhanced `ApplianceBase` class with communication statistics
- Improved error handling and debugging capabilities

### Technical Details
- Added `m_frameSuccess`, `m_frameFailure`, and `m_lastSuccessTime` member variables to `ApplianceBase`
- Success tracking increments when frames are successfully processed by AC units
- Failure tracking increments when frames timeout or are rejected
- Timestamp tracking helps monitor connection health and detect communication issues
- Fully backward compatible - existing code continues to work unchanged

### Tested
- Verified with real Midea AC hardware
- Confirmed accurate tracking of successful and failed communications
- Validated timestamp accuracy for connection health monitoring

## [1.1.9] - Previous Release
- Previous functionality maintained
