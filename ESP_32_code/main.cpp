#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>  // Core ESP32 Wi-Fi functions
#include <HTTPClient.h>  // Simple high-level HTTP helper
const char* ssid     = "Pixel_3200";
const char* password = "12345689";
const char* server   = "http://progamer228.pythonanywhere.com/api/msg/dsl001";  // Flask endpoint
Adafruit_MPU6050 mpu;

const int MAX_SIZE = 300;
float X_VAL[MAX_SIZE];
int count = 0;
String time_estimator(int ar[],int size)
{

    int local_count_ = 0;
    for(int h= 0; h<size;h++)
    {

        if(ar[h] == 1)
        {
            local_count_++;
        }

    }
    switch(local_count_)
    {
    case 1:
        return "1/3 till end(8mins)";
        break;
    case 2:
        return "2/3 till end(5mins)";
    case 3:
        return "3/3 till end(2mins)";
    case 4:
        return "4/6 ????till end";
    case 5:
        return "5/6 ????till end";
    case 6:
        return "6/6 ????till end";
    default:
        return "Estimating..";

    }
}
float measurement_error(float arr[], int size)
{
    float min_val[2]= {1000.0,1000.0};
    float max_val[2]= {-1000.0,-1000.};

    for(int i =1; i<size;i++)
    {
        if(arr[i]<min_val[1]){
            min_val[0]=min_val[1];
            min_val[1]=arr[i];
        }
        else{
            if(arr[i]<min_val[0]&& arr[i] != min_val[1])
            {
                min_val[0]=arr[i];
            }
        }

        //before the next operation save what you have had
        if(arr[i]>max_val[1]){
            max_val[0] = max_val[1];
            max_val[1]=arr[i];

        }
        else{
            if(arr[i]>max_val[0]&& arr[i] != max_val[1])
            {
                max_val[0] = arr[i];
            }
        }
    }


    return max_val[0]-min_val[0];
}
void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(500);

    }
    // Initialize MPU6050
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) {
            delay(10);
        }
    }

    // Configure accelerometer
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    delay(1000);
    Serial.println("MPU6050 ready. Collecting data...");


}

void loop() {
    const int TWO_CHECKS = 3;
    static int amount_of_twos=0;
    static const uint8_t  size_ptr=15;
    static int ptr[size_ptr];
    static bool ptr_initialized = false;
    if(!ptr_initialized)
    {
        for(int h= 0; h<(sizeof(ptr) / sizeof(ptr[0]));h++)
        {
            ptr[h] = -1;
        }
        ptr_initialized = true;
    }
    static int pos =0;
    static int  PASS_VALUES = 3;
    static int pass_start = 0, pass_middle = 0, pass_end = 0;
    bool send = false;
    String payload;



    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    if (count < MAX_SIZE) {
        X_VAL[count++] = a.acceleration.z;
        delay(14);
    } else {
        float err = measurement_error(X_VAL, count);
        Serial.print("Error: "); Serial.println(err);

        if (err <= 1.78 && err >= 0.28) {
            pass_start++;
            Serial.println("pass_start +1");
            pass_middle = pass_end = 0;
        } else if (err > 1.78) {
            pass_middle++;
            Serial.println("pass_middle +1");
            pass_start = pass_end = 0;
        } else if (err < 0.28) {
            pass_end++;
            Serial.println("pass_end +1");
            pass_start = pass_middle = 0;

        }

        if (pass_start == PASS_VALUES) {
            payload = "{\"text\":\"Laundry 1:Working/Dziala\"}";
            pass_start = 0;
            if(ptr[pos]== 1||ptr[pos]== 2)
            {
                pos+=1;
            }
            ptr[pos] = 0;
            send = true;
        } else if (pass_middle == PASS_VALUES) {
            String local_ans=time_estimator(ptr,size_ptr);
            payload = "{\"text\":\"Laundry 1:Going to be ready soon/wkrotce bedzie gotowe -- {" + local_ans + "}\"}";
            pass_middle = 0;

            if(ptr[pos]== 0||ptr[pos]== 2)
            {
                pos+=1;
            }
            ptr[pos] = 1;
            send = true;
        } else if (pass_end == PASS_VALUES+3) {
            payload = "{\"text\":\"Not working/nie dziala\"}";
            pass_end = 0;

            if(ptr[pos]== 1||ptr[pos]== 0)
            {
                pos+=1;
            }
            ptr[pos] = 2;
            send = true;
            amount_of_twos +=1;
            //cleaning buffer of messages
            if(amount_of_twos==TWO_CHECKS){
                for(int h= 0; h<(sizeof(ptr) / sizeof(ptr[0]));h++)
                {
                    ptr[h] = -1;
                }
                pos = 0;
                amount_of_twos=0;
            }
        }
        int local_count=0;
        for(int h= 0; h<(sizeof(ptr) / sizeof(ptr[0]));h++)
        {

            if(ptr[h]==1)
            {
                local_count++;
            }
        }

        if (send) {
            HTTPClient http;
            http.begin(server);
            http.addHeader("Content-Type", "application/json");
            Serial.println(payload);
            int httpCode = http.POST(payload);
            Serial.print("HTTP Response code: ");
            Serial.println(httpCode);
            http.end();
        }

        memset(X_VAL, 0, sizeof(X_VAL));
        count = 0;
        Serial.println("Array reset.");
        delay(300);
    }
}

