import heckler_bindings as h
import os
import time

script_dir = os.path.dirname(os.path.realpath(__file__))


def run_vm():
    os.system("bash " + script_dir + "/../../../run.sh")
    pass


def listen_for_events():
    errors = 0
    c = 0
    try:
        while True:
            ev = h.heckler_block_until_event()
            if ev is None or ev.get('ret') == 1 or ev.get('gpa') == 0:
                print('error')
                print(ev)
                errors = errors + 1
                if errors > 4:
                    break
                time.sleep(0.3)
                h.heckler_ack_event()
                continue
                # break
            print(hex(int(ev.get('gpa'))))
            c += 1
            h.heckler_ack_event()
            errors = 0
    except KeyboardInterrupt:
        pass
    except Exception as e:
        print(e)
        pass
    print("total: ", str(c))


def test_boot():
    print('simple init')
    h.heckler_init()

    try:
        h.heckler_enable_boot_tracking()
        listen_for_events()

    except Exception as e:
        print(e)
        pass

    h.heckler_clean_up()


def test_track_all():
    print('track all running')
    h.heckler_enable_debug()
    h.heckler_init()

    try:
        h.heckler_enable_tracking()
        h.heckler_track_all_pages()
        listen_for_events()
    except Exception as e:
        print(e)
        pass

    h.heckler_clean_up()


def test_ping_pong():
    addr1 = 0x000000000a391027
    addr2 = 0x0000000008063027

    addr1 = addr1 // 4096 * 4096
    addr2 = addr2 // 4096 * 4096

    print('addr 1', hex(addr1))
    print('addr 2', hex(addr2))
    # h.heckler_enable_debug()
    h.heckler_init()

    try:
        h.heckler_enable_tracking()
        h.heckler_track_all_pages()
        pf = 0

        errors = 0
        try:
            while True:
                ev = h.heckler_block_until_event()
                if ev is None or ev.get('ret') == 1 or ev.get('gpa') == 0:
                    print('error')
                    print(ev)
                    break
                print(hex(int(ev.get('gpa'))))
                gpa = int(ev.get('gpa'))
                if gpa == addr1:
                    pf += 1
                    print('got addr 1')
                    h.heckler_track_page(addr2)
                elif gpa == addr2:
                    print('got addr 2')
                    pf += 1
                    h.heckler_track_page(addr1)

                h.heckler_ack_event()
                errors = 0
        except KeyboardInterrupt:
            pass
        except Exception as e:
            print(e)
            pass
        print('got pfs: ', pf)

        h.heckler_clean_up()
    except Exception as e:
        print(e)


def test_all():
    a1 = 0x0000000052b14000
    a2 = 0x0000000051c2d000

    h.heckler_init()
    last = 0

    try:
        h.heckler_enable_tracking()
        pf = 0

        h.heckler_track_page(a1)
        h.heckler_track_page(a2)

        errors = 0
        try:
            while True:
                ev = h.heckler_block_until_event()
                if ev is None or ev.get('ret') == 1 or ev.get('gpa') == 0:
                    print('error')
                    print(ev)
                    break
                print(hex(int(ev.get('gpa'))))
                gpa = int(ev.get('gpa'))

                print(gpa)
                if last != 0 and last != gpa:
                    h.heckler_track_page(last)
                last = gpa
                pf += 1

                h.heckler_ack_event()
                errors = 0
        except KeyboardInterrupt:
            pass
        except Exception as e:
            print(e)
            pass
        print('got pfs: ', pf)

        h.heckler_clean_up()
    except Exception as e:
        print(e)


if __name__ == "__main__":
    test_boot()
    # test_all()
    # test_ping_pong()
    # test_track_all()
    # run_vm()
