struct MandM
{
public:
    MandM(float redmin, float redmax, float greenmin, float greenmax, float bluemin, float bluemax); // constructor
    ~MandM();                                                                                        // destructor

    bool IsMatch(float &red, float &green, float &blue);

private:
    float red_min;
    float red_max;
    float green_min;
    float green_max;
    float blue_min;
    float blue_max;
};

MandM::MandM(float redmin, float redmax, float greenmin, float greenmax, float bluemin, float bluemax)
{
    red_min = redmin;
    red_max = redmax;
    green_min = greenmin;
    green_max = greenmax;
    blue_min = bluemin;
    blue_max = bluemax;
}

bool MandM::IsMatch(float &red, float &green, float &blue)
{
    // char buffer[256];

    // Logger.Error("WTF:");
    // snprintf(buffer, sizeof(buffer), "R: %.0f G: %.0f B: %.0f", red, green, blue);
    // Logger.Info(buffer);

    // snprintf(buffer, sizeof(buffer), "Redmin: %.0f Redmax: %.0f", red_min, red_max);
    // Logger.Info(buffer);

    // snprintf(buffer, sizeof(buffer), "Greenmin: %.0f Greenmax: %.0f", green_min, green_max);
    // Logger.Info(buffer);

    // snprintf(buffer, sizeof(buffer), "Bluemin: %.0f Bluemax: %.0f", blue_min, blue_max);
    // Logger.Info(buffer);

    return (red >= red_min && red <= red_max) &&
           (green >= green_min && green <= green_max) &&
           (blue >= blue_min && blue <= blue_max);
}