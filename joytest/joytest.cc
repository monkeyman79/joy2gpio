#include <linux/joystick.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <map>

using namespace std;

int main(int argc, char** argv)
{
    int axis = 0;
    if (argc < 2)
    {
        printf("Usage: %s /dev/input/js0 [axis]\n", argv[0]);
        return 1;
    }

    if (argc == 3)
    {
        axis = atol(argv[2]);
    }

    int fd = open(argv[1], O_RDONLY, 0);
    if (fd < 0)
    {
        perror(argv[1]);
        return 1;
    }

    js_event ev;
    map<int, bool> buttons;
    map<int, int> axes;
    while (read(fd, &ev, sizeof(ev)) == sizeof(ev))
    {
        if ((ev.type & ~JS_EVENT_INIT) == JS_EVENT_BUTTON)
        {
            buttons[ev.number] = ev.value ? true : false;
        }
        else if ((ev.type & ~JS_EVENT_INIT) == JS_EVENT_AXIS)
        {
            axes[ev.number] = ev.value;
        }
        printf("\x1b[H");
        for (auto& btn : buttons)
        {
            printf("Button #%d: %s\x1b[K\n", btn.first, btn.second ? "ON" : "OFF");
        }
        for (auto& ax : axes)
        {
            printf("Axis #%d: %d\x1b[K\n", ax.first, ax.second);
        }
        if (axes.count(axis) && axes.count(axis+1))
        {
            int x = axes[axis] * 20 / 32767 + 20 + 1;
            int y = axes[axis+1] * 10 / 32767 + 10 + 1;
            printf("\x1b[%d;%dH", y, x);
            fflush(stdout);
        }
    }
}

