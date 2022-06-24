using Newtonsoft.Json;
using Plugin.BluetoothLE;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using System.Timers;
using Xamarin.Forms;
using JonnyGlasses1._0.Models;
using Xamarin.Essentials;

namespace JonnyGlasses1._0
{
    public partial class MainPage : ContentPage
    {
        //bluetooth vars
        IDevice _connectedDevice;
        int count = 0;
        bool started_scan = false;

        //weather vars
        private static Timer weatherTimer;
        private string location = "Comox";
        OpenWeather currentWeather;
        List<OpenWeather> weatherList;

        public MainPage()
        {
            InitializeComponent();
            currentWeather = new OpenWeather();
            weatherList = new List<OpenWeather>();
            weatherTimer = new Timer(5000);
            weatherTimer.Elapsed += WeatherTimer_Elapsed;
            weatherTimer.AutoReset = true;
            weatherTimer.Enabled = true;
        }

        public static async Task<dynamic> getDataFromService(string pQueryString)
        {
            HttpClient client = new HttpClient();
            var response = await client.GetAsync(pQueryString);
            dynamic data = null;
            if (response != null)
            {
                string json = response.Content.ReadAsStringAsync().Result;
                data = JsonConvert.DeserializeObject(json);
            }
            return data;
        }

        private async void WeatherTimer_Elapsed(object sender, ElapsedEventArgs e)
        {
            var location = (await Geocoding.GetLocationsAsync("Comox")).FirstOrDefault();
            string queryString = $"https://api.openweathermap.org/data/2.5/onecall?lat={location.Latitude}&lon={location.Longitude}&exclude=minutely,hourly&appid=dba70123e89fa1898befb2674ef1a964&units=metric";
            dynamic weatherResults = await getDataFromService(queryString).ConfigureAwait(false);
            if (weatherResults["current"] != null)
            {
                currentWeather.Temperature = (string)weatherResults["current"]["temp"] + " C";
                DateTime time = new System.DateTime(1970, 1, 1, 0, 0, 0, 0);
                DateTime currentTime = DateTime.Now;
                DateTime sunrise = time.AddSeconds((double)weatherResults["current"]["sunrise"]);
                DateTime sunset = time.AddSeconds((double)weatherResults["current"]["sunset"]);
                currentWeather.Sunrise = sunrise.ToString() + " UTC";
                currentWeather.Sunset = sunset.ToString() + " UTC";
                currentWeather.Wind = (string)weatherResults["current"]["wind_speed"] + " kmh";
                currentWeather.Humidity = (string)weatherResults["current"]["humidity"] + " %";
                currentWeather.weatherID = (string)weatherResults["current"]["weather"][0]["id"];
                Console.WriteLine(currentTime.ToString());
                weatherList.Clear();
                for(int i = 0; i < 7; i++)
                {
                    OpenWeather dayWeather = new OpenWeather();
                    dayWeather.Temperature = (string)weatherResults["daily"][i]["temp"]["min"] + " C - " + (string)weatherResults["daily"][i]["temp"]["max"] + " C";
                    dayWeather.Wind = (string)weatherResults["daily"][i]["wind_speed"] + " kmh";
                    dayWeather.Humidity = (string)weatherResults["daily"][i]["humidity"] + " %";
                    dayWeather.weatherID = (string)weatherResults["daily"][i]["weather"][0]["id"];
                    weatherList.Add(dayWeather);
                }
            }
        }

        private async void Connect(object sender, EventArgs e)
        {
            if (!started_scan)
            {
                ScanConfig config = new ScanConfig();
                Guid guid = new Guid("beb5483e-36e1-4688-b7f5-ea07361b26a8");
                config.ServiceUuids = new List<Guid> { guid };
                var _adapter = CrossBleAdapter.Current;
                _adapter.ScanForUniqueDevices(config).Subscribe(device => DeviceDiscovered(device));
                started_scan = true;
            }
        }

        private void DeviceDiscovered(IDevice device)
        {
            Console.Write(device.Name);
            //_connectedDevice = device;
            //_connectedDevice.WhenAnyCharacteristicDiscovered().Subscribe(result => CharacteristicsDiscovered(result));
            //_connectedDevice.Connect();
        }

        private void CharacteristicsDiscovered(IGattCharacteristic result)
        {
            Console.Write(result.Uuid);
        }

        private void Disconnect(object sender, EventArgs e)
        {
            if (_connectedDevice!=null && _connectedDevice.IsConnected())
                _connectedDevice.CancelConnection();
        }
    }
}
