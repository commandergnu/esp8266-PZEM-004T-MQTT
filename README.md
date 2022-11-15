# esp8266-PZEM-004T-MQTT

### openhab



#### .items

String ESP_Watmeter_status       "Status"                <switch>     (ESP_Watmeter_Group)    {channel="mqtt:topic:ESP_Watmeter:status"}

Group  ESP_Watmeter_Group        "ESP_Watmeter_Group"    <energy>     (All)
Number ESP_Watmeter_Volt         "Voltage     [%d V]"    <pressure>   (ESP_Watmeter_Group)    {channel="mqtt:topic:ESP_Watmeter_voltage:volt"}
Number ESP_Watmeter_Current      "Current     [%s A]"    <pressure>   (ESP_Watmeter_Group)    {channel="mqtt:topic:ESP_Watmeter_current:current"}
Number ESP_Watmeter_Power        "Power       [%d W]"    <pressure>   (ESP_Watmeter_Group)    {channel="mqtt:topic:ESP_Watmeter_power:power"}
Number ESP_Watmeter_Energy       "Energy      [%s Kwh]"  <pressure>   (ESP_Watmeter_Group)    {channel="mqtt:topic:ESP_Watmeter_energy:energy"}
Number ESP_Watmeter_Frequency    "Frequency   [%d Hz]"   <pressure>   (ESP_Watmeter_Group)    {channel="mqtt:topic:ESP_Watmeter_frequency:frequency"}
Number ESP_Watmeter_Pf           "PowerFactor [%s ]"     <pressure>   (ESP_Watmeter_Group)    {channel="mqtt:topic:ESP_Watmeter_pf:pf"}
Switch ESP_Watmeter_Switch       "PowerReserve"          <pressure>   (ESP_Watmeter_Group)    {channel="mqtt:topic:ESP_Watmeter_Switch"}



#### .things

Thing mqtt:topic:ESP_Watmeter "ESP_Watmeter" (mqtt:broker:33513b1646) [availabilityTopic="ESP_Watmeter/status/LWT", payloadAvailable="Online", payloadNotAvailable="Offline"]


   {
    Channels:
           Type string : status       "ESP Watmeter"     [stateTopic="ESP_Watmeter/status/LWT"]
           Type number : Voltage      "Voltage"          [stateTopic="ESP_Watmeter_voltage"]
           Type number : Current      "Current"          [stateTopic="ESP_Watmeter_current"]
           Type number : Power        "Power"            [stateTopic="ESP_Watmeter_power"]
           Type number : Energy       "Energy"           [stateTopic="ESP_Watmeter_energy"]
           Type number : Frequency    "Frequency"        [stateTopic="ESP_Watmeter_frequency"]
           Type number : Pf           "Pf"               [stateTopic="ESP_Watmeter_pf"]
           Type switch : ESP_Watmeter "PowerReserve"     [stateTopic="ESP_Watmeter_State", commandTopic="ESP_Watmeter_Switch", on="1", off="0"]

}


#### .sitemap

 Frame label=Watmeter{
            Text label="Watmeter" icon="energy"
            Text item=ESP_Watmeter_status       icon="Switch"   valuecolor=[Online="lime", Offline="red"]
            Switch item=ESP_Watmeter_Switch     mappings=[OFF="OFF",ON="ON"]  icon="Switch"   valuecolor=[Online="lime", Offline="red"]
            Text item=ESP_Watmeter_Volt         icon="Pressure"
            Text item=ESP_Watmeter_Current      icon="Pressure"
            Text item=ESP_Watmeter_Power        icon="Pressure"
            Text item=ESP_Watmeter_Energy       icon="Pressure"
            Text item=ESP_Watmeter_Frequency    icon="Pressure"
            Text item=ESP_Watmeter_Pf           icon="Pressure"

          }



#####


