cat > telemetry.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int get_battery() {
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

int get_cpu_temp() {
    FILE *fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (!fp) return -1;
    int temp;
    fscanf(fp, "%d", &temp);
    fclose(fp);
    return temp / 1000;
}

int main() {
    printf("\n⚡ MARDUKH TELEMETRY (Real Sensor Data)\n");
    printf("Reading battery and CPU temperature every 5 seconds.\n");
    printf("Press Ctrl+C to stop.\n\n");

    while (1) {
        int battery = get_battery();
        int temp = get_cpu_temp();

        time_t now;
        time(&now);
        struct tm *tm_info = localtime(&now);
        
        printf("[%02d:%02d:%02d] 🔋 Battery: %d%% | 🌡️ CPU: %d°C\n",
               tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec, battery, temp);

        sleep(5);
    }
    return 0;
}
EOF
