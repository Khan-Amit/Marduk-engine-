cat > telemetry.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define JSON_FILE "telemetry_state.json"

int main() {
    printf("\n⚡ MARDUKH TELEMETRY (Termux)\n");
    printf("Reading telemetry_state.json every 5 seconds.\n");
    printf("Press Ctrl+C to stop.\n\n");

    while (1) {
        FILE *json = fopen(JSON_FILE, "r");
        int voip = 0, drop = 0;
        double gain = 0.0;

        if (json) {
            char line[256];
            while (fgets(line, sizeof(line), json)) {
                if (strstr(line, "\"voip_ingress_kbps\"")) {
                    char *colon = strchr(line, ':');
                    if (colon) voip = atoi(colon + 1);
                }
                if (strstr(line, "\"filtration_drop_rate\"")) {
                    char *colon = strchr(line, ':');
                    if (colon) drop = atoi(colon + 1);
                }
                if (strstr(line, "\"financial_gain_usd\"")) {
                    char *colon = strchr(line, ':');
                    if (colon) gain = atof(colon + 1);
                }
            }
            fclose(json);
        } else {
            printf("⚠️ telemetry_state.json not found.\n");
        }

        time_t now;
        time(&now);
        struct tm *tm_info = localtime(&now);
        printf("[%02d:%02d:%02d] VoIP: %d kbps | Drop: %d%% | Gain: $%.2f\n",
               tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec, voip, drop, gain);

        sleep(5);
    }
    return 0;
}
EOF
