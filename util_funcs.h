#ifndef UTIL_FUNCS_H
#define UTIL_FUNCS_H
void DilateErode(cv::Mat thresholdedImage)
{
    ////morphological opening (removes small objects from the foreground)
    erode(thresholdedImage, thresholdedImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    dilate(thresholdedImage, thresholdedImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

    //morphological closing (removes small holes from the foreground)
    dilate(thresholdedImage, thresholdedImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    erode(thresholdedImage, thresholdedImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

}

//Could be two functions, but wanted to use std::pair and std::tie to get results
std::pair <std::vector<std::vector<cv::Point>>, std::vector<cv::Vec3i>> getContoursAndCircles(cv::Mat imageToSearchContoursIn)
{

    //******************************************************************************************************************************************************
    //Finding circles - one way
    std::vector<std::vector<cv::Point>> contouredElementsOnScreen;
    std::vector<cv::Vec3i> circlesFoundUsingContours;


    //cv::findContours(canny_out, conturedElementsOnScreen, cv::RetrievalModes::RETR_EXTERNAL, cv::ContourApproximationModes::CHAIN_APPROX_NONE);
    cv::findContours(imageToSearchContoursIn, contouredElementsOnScreen, cv::RetrievalModes::RETR_EXTERNAL, cv::ContourApproximationModes::CHAIN_APPROX_NONE);



    //Discard all the elements from the list, for which the contour is smaller than {X} points
    //We are only interested in big objects
    unsigned int minPointsInContour = 50;

    //Discarding using C++ iterators
    //for (auto it = conturedElementsOnScreen.begin(); it < conturedElementsOnScreen.end(); )
    //{
    //	auto contour = *it;
    //	if (contour.size() < minPointsInContour)
    //	{
    //		it = conturedElementsOnScreen.erase(it);
    //	}
    //	else  ++it;
    //}

    //Discarding using C++11 std::remove_if and lambda expressions
    contouredElementsOnScreen.erase(std::remove_if(contouredElementsOnScreen.begin(),
        contouredElementsOnScreen.end(),
        [=](const std::vector<cv::Point>& contour)
    {
        if (contour.size() < minPointsInContour) return true;
        else { return false; }
    }),
        contouredElementsOnScreen.end());

    //Iterate over what's left (only big contours) and try to find all the circles

    for (auto contour : contouredElementsOnScreen)
    {

        cv::Moments tempMoments = cv::moments(contour);
        //Moments
        //double dM02 = tempMoments.m02;
        //double dM20 = tempMoments.m20;
        //Normalized moments
        double dN02 = tempMoments.nu02;
        double dN20 = tempMoments.nu20;

        double area = tempMoments.m00;
        double radius = std::sqrt(area / 3.141592);

        double maxMoment = std::max(dN02, dN20);
        double minMoment = std::min(dN02, dN20);

        if ((maxMoment / minMoment) > 0.75 && (maxMoment / minMoment) < 1.25)
        {
            std::cout << "Found circle  radius is " <<radius<< std::endl;
            int posY = (int)(tempMoments.m01 / tempMoments.m00);
            int posX = (int)(tempMoments.m10 / tempMoments.m00);


            if (posX > 0 && posY > 0)
            {
                circlesFoundUsingContours.push_back(cv::Vec3i(posX, posY, (int)radius));
            }
        }

    }

    //return std::make_pair(contouredElementsOnScreen, circlesFoundUsingContours);
    return {contouredElementsOnScreen, circlesFoundUsingContours};

}

std::vector<cv::Vec3f> findCirclesUsingHoughes(cv::Mat imageToSearchCirclesIn)
{
    std::vector<cv::Vec3f> circlesFoundUsingHoughes;

    //Hough Transform - detecting circles
    //Blur the image before - improves detection
    cv::GaussianBlur(imageToSearchCirclesIn, imageToSearchCirclesIn, cv::Size(9, 9), 2, 2);
    cv::HoughCircles(imageToSearchCirclesIn, circlesFoundUsingHoughes, cv::HOUGH_GRADIENT, 2, imageToSearchCirclesIn.rows / 4, 200, 50);


    return circlesFoundUsingHoughes;

}


cv::Point findCenterOfBlobUsingMoments(cv::Mat imageToSearchBlobIn)
{

    cv::Point blobCenter = { -1,-1 };
    //Calculate the moments of the thresholded image
    cv::Moments oMoments = cv::moments(imageToSearchBlobIn);

    double dM01 = oMoments.m01;
    double dM10 = oMoments.m10;
    double dArea = oMoments.m00;


    // if the area of the found object is not big enough, then it's just noise
    if (dArea > 10000)
    {
        //calculate the position of the blob
        int posX = (int)(dM10 / dArea);
        int posY = (int)(dM01 / dArea);

        if (posX >= 0 && posY >= 0)
        {
            blobCenter = cv::Point{ posX, posY };

        }

    }

    return blobCenter;
}
#endif // UTIL_FUNCS_H
