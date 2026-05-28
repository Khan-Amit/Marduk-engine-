#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Function to read battery level from Termux API
int get_battery_level() {
    FILE *fp = popen("termux-battery-status 2>/dev/null | grep 'percentage' | head -1", "r");
    if (!fp) return -1;
    char line[64];
    int level = -1;
    if (fgets(line, sizeof(line), fp)) {
        char *colon = strchr(line, ':');
        if (colon) level = atoi(colon + 1);
    }
    pclose(fp);
    return level;
}

// Function to read CPU temperature (first thermal zone)
int get_cpu_temp() {
    FILE *fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (!fp) return -1;
    int temp;
    fscanf(fp, "%d", &temp);
    fclose(fp);
    return temp / 1000; // Convert millidegrees to Celsius
}

// Function to read total received bytes from network (simplified)
long get_network_rx() {
    FILE *fp = fopen("/proc/net/dev", "r");
    if (!fp) return -1;
    char line[256];
    long rx = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "wlan0") || strstr(line, "eth0")) {
            char *ptr = line;
            while (*ptr && !(*ptr >= '0' && *ptr <= '9')) ptr++;
            rx = atol(ptr);
            break;
        }
    }
    fclose(fp);
    return rx;
}

int main() {
    printf("\n⚡ MARDUKH TELEMETRY (Real Sensor Data)\n");
    printf("Reading from phone sensors every 5 seconds.\n");
    printf("Press Ctrl+C to stop.\n\n");

    while (1) {
        int battery = get_battery_level();
        int temp = get_cpu_temp();
        long rx = get_network_rx();

        time_t now;
        time(&now);
        struct tm *tm_info = localtime(&now);
        
        printf("[%02d:%02d:%02d] 🔋 Battery: %d%% | 🌡️ CPU: %d°C | 📡 Network RX: %ld bytes\n",
               tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec, battery, temp, rx);

        sleep(5);
    }
    return 0;
}
