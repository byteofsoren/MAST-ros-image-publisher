#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream> // Converts the inline parameters to int. 

int main(int argc, char *argv[])
{
    if (argv == NULL) {
        return 1;
    }
    ros::init(argc,argv,"image_publisher");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher ros_pub_handle = it.advertise("camera/image",1);

    // Convert the passed as commandline parameter inde for the video device to an integer
    std::istringstream video_soureCemd(argv[1]);
    int video_source;
    if (!(video_soureCemd >> video_source)) return 1;

    cv::VideoCapture cap(video_source);
    // Check if the video can be opened with the given index.
    if(!cap.isOpened()) return 1;
    cv::Mat frame;
    sensor_msgs::ImagePtr msg;
    ros::Rate loop_rate(5);
    while (nh.ok()){
        cap >> frame;
        // Check if grabbed frame is acually full with some content.
        if (!frame.empty()) {
            msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
            ros_pub_handle.publish(msg);
            cv::waitKey(1);
        }
        ros::spinOnce();
        loop_rate.sleep();
    }



    return 0;
}
