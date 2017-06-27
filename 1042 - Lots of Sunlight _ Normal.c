/*1042 - Lots of Sunlight*/
#include <stdio.h>
#include <math.h>
#define M_PI 3.14159265358979323846
const int morning = 5*60*60 + 37*60;
const int night = 18*60*60 + 17*60;
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, k, l;
    int caseNumber = 1;
    while(1){
        int buildingNumber;
        scanf("%d", &buildingNumber);
        if(buildingNumber == 0) break;

        int width, height;
        scanf("%d %d", &width, &height);

        int buildings[buildingNumber];
        int distance[buildingNumber-1];
        for(i = 0; i < buildingNumber; i++){
            scanf("%d", &buildings[i]);
            if(i < buildingNumber-1)scanf("%d", &distance[i]);
        }

        printf("Apartment Complex: %d\n", caseNumber);
        int query;
        while(1){
            scanf("%d", &query);
            if(query == 0) break;
            int f = query/100, b = query%100;
            /*printf("B: %d, F: %d VS Building: %d, Floor: %d\n", b, f, buildingNumber, buildings[b-1]);*/
            if(b > buildingNumber || f > buildings[b-1] || b == 0 || f == 0) {
                printf("Apartment %d: Does not exist\n", query);
                continue;
            }
            /*Calculate left angle and translate it to time*/
            char left[15] = {'\0'};
            /*Find the max angle int left part*/
            double y1 = (f-1)*height;
            double x1 = width*(b-1);
            for(j = 0; j < b-1; j++) x1 += distance[j];
            double m = 0;
            for(i = 0; i < b-1; i++){
                double y2 = height*buildings[i];
                double x2 = width*(i+1);
                for(j = 0; j < i; j++) x2 += distance[j];
                if( (y2 - y1)/(x1 - x2) > m ) m = (y2 - y1)/(x1 - x2);
            }
            int lt = atan(m)/M_PI *(night - morning) + morning;
            sprintf(left, "%02d:%02d:%02d", lt/3600, (lt%3600)/60, (lt%60));

            /*Calculate right angle and translate it to time*/
            char right[15] = {'\0'};
            /*Find the max angle int right part*/
            x1 += width;
            m = 0;
            for(i = b; i < buildingNumber; i++){
                double y2 = height*buildings[i];
                double x2 = width*(i);
                for(j = 0; j < i; j++) x2 += distance[j];
                if( (y2 - y1)/(x2 - x1) > m ) m = (y2 - y1)/(x2 - x1);
            }
            int rt = (1-atan(m)/M_PI) *(night - morning) + morning;
            sprintf(right, "%02d:%02d:%02d", rt/3600, (rt%3600)/60, (rt%60));

            printf("Apartment %d: %s - %s\n", query, left, right);
        }
        caseNumber++;
    }
    return 0;
}
