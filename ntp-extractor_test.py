from sqlite3 import Timestamp
import time
import numpy as np
import cv2
import pytz
import time
import datetime
from ntp_extractor import VideoCap

# def convert2UTC( timestamp):
#         return str(datetime.datetime.fromtimestamp(timestamp, tz=pytz.UTC).strftime("%Y-%m-%d %H:%M:%S.%f"))

if __name__ == "__main__":

    # rtsp url (Has RTCP Sendor Report)
    url = "rtsp://admin:admin123@192.168.1.47:554/"
    cap = VideoCap()
    # open the url
    ret = cap.open(url)

    if not ret:
        raise RuntimeError("Could not open the video url")

    print("Sucessfully opened video file")
    count = 0
    # continuously read and display video frames and motion vectors
    while True: 
        # read next video frame and corresponding frame_timstampe
        ret, frame, timestamp = cap.read()
        # convert time to UTC time
        # timestamp_utc = convert2UTC(timestamp)
        count+=1
        # cv2.putText(frame, "TIME RECEIVED "+str(timestamp_utc), (250,250), cv2.FONT_HERSHEY_SIMPLEX,2, (0,255,0), 4, cv2.LINE_AA)
        # cv2.imwrite(f'./output/{time.time()}.jpg', frame)  
        # print results
        print("Frame: {} | ".format(count), end=" ")
        print("Timestamp: {} | ".format(timestamp))
    cap.release()

