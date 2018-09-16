/**
 * Based on alsactl/monitor.c (by Takashi Iwai <tiwai@suse.de>) from alsa-utils
 * http://git.alsa-project.org/?p=alsa-utils.git;a=blob_plain;f=alsactl/monitor.c;hb=HEAD
 *
 * Inspired by kbdd
 */

#include <stdio.h>
#include <alsa/asoundlib.h>

int notify()
{
    fprintf(stderr, "valueChanged\n");
    return 0;
}

int open_ctl(const char *name, snd_ctl_t **ctlp)
{
    snd_ctl_t *ctl;
    int err;

    err = snd_ctl_open(&ctl, name, SND_CTL_READONLY);
    if (err < 0) {
        fprintf(stderr, "Cannot open ctl %s\n", name);
        return err;
    }
    err = snd_ctl_subscribe_events(ctl, 1);
    if (err < 0) {
        fprintf(stderr, "Cannot open subscribe events to ctl %s\n", name);
        snd_ctl_close(ctl);
        return err;
    }
    *ctlp = ctl;
    return 0;
}

int check_event(snd_ctl_t *ctl)
{
    snd_ctl_event_t *event;
    unsigned int mask;
    int err;

    snd_ctl_event_alloca(&event);
    err = snd_ctl_read(ctl, event);
    if (err < 0)
        return err;

    if (snd_ctl_event_get_type(event) != SND_CTL_EVENT_ELEM)
        return 0;

    mask = snd_ctl_event_elem_get_mask(event);
    if (!(mask & SND_CTL_EVENT_MASK_VALUE))
        return 0;

    notify();

    return 0;
}

#define MAX_CARDS   256

int monitor(const char *name) {
    snd_ctl_t *ctl;
    int err = 0;

    err = open_ctl(name, &ctl);
    if (err < 0)
        goto error;

    while (1) {
        struct pollfd fd;

        snd_ctl_poll_descriptors(ctl, &fd, 1);

        err = poll(&fd, 1, -1);
        if (err <= 0) {
            err = 0;
            break;
        }

        unsigned short revents;
        snd_ctl_poll_descriptors_revents(ctl, &fd, 1,
                         &revents);
        if (revents & POLLIN)
            check_event(ctl);
    }

error:
    snd_ctl_close(ctl);
    return err;
}

void usage()
{
    fprintf(stderr,
        "Usage:\n"
        "   alsa-volume-monitor <card name>\n\n"
        "Example:\n"
        "   alsa-volume-monitor hw:0\n");
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        usage();

    monitor(argv[1]);

    return 0;
}
