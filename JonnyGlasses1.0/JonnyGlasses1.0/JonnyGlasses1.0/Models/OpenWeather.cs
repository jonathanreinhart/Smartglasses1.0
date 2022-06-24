using System;
using System.Collections.Generic;
using System.Text;

namespace JonnyGlasses1._0.Models
{
    public class OpenWeather
    {
        public string Temperature
        {
            get;
            set;
        }
        public string Wind
        {
            get;
            set;
        }
        public string Humidity
        {
            get;
            set;
        }
        public string Visibility
        {
            get;
            set;
        }
        public string Sunrise
        {
            get;
            set;
        }
        public string Sunset
        {
            get;
            set;
        }
        public string weatherID
        {
            get;
            set;
        }
        public OpenWeather()
        {
            this.Temperature = " ";
            this.Wind = " ";
            this.Humidity = " ";
            this.Visibility = " ";
            this.Sunrise = " ";
            this.Sunset = " ";
            this.weatherID = " ";
        }
    }
}
