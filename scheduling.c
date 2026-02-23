#include <stdio.h>
#define MAX 20

int main()
{
    int n;
    int p[MAX], a[MAX], b[MAX], pr[MAX];
    int ct[MAX], tat[MAX], wt[MAX];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for(int i = 0; i < n; i++)
    {
        printf("\nProcess %d\n", i+1);
        printf("Enter Process ID: ");
        scanf("%d", &p[i]);
        printf("Arrival Time: ");
        scanf("%d", &a[i]);
        printf("Burst Time: ");
        scanf("%d", &b[i]);
        printf("Priority: ");
        scanf("%d", &pr[i]);
    }

    int choice;
    do
    {
        printf("\nChoose Scheduling Method (0 to Exit):");
        printf("\n1. FCFS");
        printf("\n2. SJF (Non-Preemptive)");
        printf("\n3. Priority (Non-Preemptive)");
        printf("\n4. Round Robin (q=3)");
        printf("\n0. Exit");
        printf("\nEnter choice: ");
        scanf("%d", &choice);

        float total_wt = 0;

        switch(choice)
        {
            case 0:
                printf("Exiting program.\n");
                break;

            case 1: // FCFS
            {
                // Sort by arrival time
                for(int i = 0; i < n-1; i++)
                {
                    for(int j = i+1; j < n; j++)
                    {
                        if(a[i] > a[j])
                        {
                            int temp;
                            temp = a[i]; a[i] = a[j]; a[j] = temp;
                            temp = b[i]; b[i] = b[j]; b[j] = temp;
                            temp = p[i]; p[i] = p[j]; p[j] = temp;
                            temp = pr[i]; pr[i] = pr[j]; pr[j] = temp;
                        }
                    }
                }

                int time = 0;
                for(int i = 0; i < n; i++)
                {
                    if(time < a[i])
                        time = a[i];

                    ct[i] = time + b[i];
                    tat[i] = ct[i] - a[i];
                    wt[i] = tat[i] - b[i];
                    total_wt += wt[i];
                    time = ct[i];
                }
                break;
            }

            case 2: // SJF Non-Preemptive
            {
                int done[MAX] = {0}, complete = 0, time = 0;

                while(complete < n)
                {
                    int min = 9999, idx = -1;
                    for(int i = 0; i < n; i++)
                    {
                        if(a[i] <= time && !done[i] && b[i] < min)
                        {
                            min = b[i];
                            idx = i;
                        }
                    }
                    if(idx == -1){ time++; continue; }

                    ct[idx] = time + b[idx];
                    tat[idx] = ct[idx] - a[idx];
                    wt[idx] = tat[idx] - b[idx];
                    total_wt += wt[idx];

                    time = ct[idx];
                    done[idx] = 1;
                    complete++;
                }
                break;
            }

            case 3: // Priority Non-Preemptive
            {
                int done[MAX] = {0}, complete = 0, time = 0;

                while(complete < n)
                {
                    int max = -1, idx = -1;
                    for(int i = 0; i < n; i++)
                    {
                        if(a[i] <= time && !done[i] && pr[i] > max)
                        {
                            max = pr[i];
                            idx = i;
                        }
                    }
                    if(idx == -1){ time++; continue; }

                    ct[idx] = time + b[idx];
                    tat[idx] = ct[idx] - a[idx];
                    wt[idx] = tat[idx] - b[idx];
                    total_wt += wt[idx];

                    time = ct[idx];
                    done[idx] = 1;
                    complete++;
                }
                break;
            }

            case 4: // Round Robin
            {
                int rt[MAX], queue[1000];
                int front=0, rear=0, visited[MAX]={0}, time=0, complete=0, quantum=3;

                for(int i=0;i<n;i++) rt[i]=b[i];

                while(complete<n)
                {
                    for(int i=0;i<n;i++)
                        if(a[i]<=time && !visited[i]) { queue[rear++]=i; visited[i]=1; }

                    if(front==rear){ time++; continue; }

                    int idx=queue[front++];
                    int run=(rt[idx]>quantum)?quantum:rt[idx];
                    rt[idx]-=run;
                    time+=run;

                    for(int i=0;i<n;i++)
                        if(a[i]<=time && !visited[i]) { queue[rear++]=i; visited[i]=1; }

                    if(rt[idx]>0) queue[rear++]=idx;
                    else
                    {
                        ct[idx]=time;
                        tat[idx]=ct[idx]-a[idx];
                        wt[idx]=tat[idx]-b[idx];
                        total_wt+=wt[idx];
                        complete++;
                    }
                }
                break;
            }

            default:
                printf("Invalid choice! Try again.\n");
                continue;
        }

        if(choice >= 1 && choice <= 4)
        {
            printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
            for(int i=0;i<n;i++)
                printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i], a[i], b[i], ct[i], tat[i], wt[i]);
            printf("\nAverage Waiting Time = %.2f\n", total_wt/n);
        }

    } while(choice != 0);

    return 0;
}
