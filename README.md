# Prometheus Server Setup and Troubleshooting

## Overview
A demonstration of using Prometheus for monitoring metrics in a C++ client-server application. This repository includes a simple example of how to integrate the Prometheus C++ client library to expose metrics, allowing monitoring of custom metrics such as request counts and response times. The example showcases both server-side and client-side implementations, with detailed setup and configuration instructions.

## Getting Started

### Prerequisites

- **Prometheus Version**: 2.54.0-rc.0

### Installation

1. **Download and Install Prometheus**:
   Download the Prometheus binary from the [Prometheus releases page](https://prometheus.io/download/).

2. **Extract and Setup**:
   ```bash
   tar -xvf prometheus-*.tar.gz
   cd prometheus-*
   ```
3. **Start Prometheus**:
   ```bash
   ./prometheus --config.file=prometheus.yml
   ```
### Build

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```
### Run Server and Client

   ```bash
   ./server
   ./client
   ```

### Usage

1. **Access Prometheus Web UI**:
Open your browser and navigate to http://HostIP:9090 to access the Prometheus web interface.

2. **Verify Data Collection**:
Use the web interface to query and visualize metrics collected from your targets.


