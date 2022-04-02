# ntp-extractor
Used to extract frames and timestamp of each frame from H.264 and MPEG-4 encoded.


## Installation

### Step 1: Install Prerequisites

Install [Docker](https://docs.docker.com/).

### Step 2: Clone Source Code

Go to your installation directory and clone the source code:
```
git clone https://github.com/nmditai/ntp-extractor.git ntp_extractor
```

### Step 3: Build Images And Run

Move into folder **/ntp_extractor**  and proceed to build images:
```
sudo docker build -t ntp_extractor .
```
Run the container with the newly built images:
```
docker run -it ntp-extractor:latest /bin/bash
```


### Step 4: Test

In the file ntp-extractor_test.py, please fix the url with RTSP link and make sure it supports RTCP Sendor Report, run:
```
python3 ntp-extractor_test.py
```
