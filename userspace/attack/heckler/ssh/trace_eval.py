import json

from ordered_set import OrderedSet

from heckler.common import load_json


#
# XXX: Accuracy of attack depends on this function
# given profiling data from offline analysis, find a pattern in a new trace
# to determine gadget page:
# mm_answer_authpassword -> auth_password .... -> mm_answer_authpassword
#
# the values used here are highly CVM specific and need fine tuning for a new setup
#
def find_candidates(run_list, run_set, gt_auth_password, gt_mm_answer_authpassword, debug=False):

    c_direct = _find_direct_backjump(
        run_list=run_list,
        run_set=run_set,
        gt_auth_password=gt_auth_password,
        gt_mm_answer_authpassword=gt_mm_answer_authpassword,
        debug=debug)

    c_indirect = _find_no_direct_backjump(
        run_list=run_list,
        run_set=run_set,
        gt_auth_password=gt_auth_password,
        gt_mm_answer_authpassword=gt_mm_answer_authpassword,
        debug=debug)

    candidates = c_indirect

    if debug:
        show_hint(candidates,
                  run_list=run_list,
                  gt_auth_password=gt_auth_password,
                  gt_mm_answer_authpassword=gt_mm_answer_authpassword,
                  debug=True)

    return candidates


# XXX: this approach ignores the back jump from
# auth_password -> mm_answer_authpassword and tries to find the gadgets
#
# for some CVM, we do not find a direct backjump but a strange
# ping pong pattern of auth_password and some other page even though the backjump
# should only be occurring once in a perfect trace sequence
# mm_answer_authpassword -> auth_password .... backjump: auth_password -> mm_answer_authpassword.
#
# instead, auth_password ping pongs with some other page until max_hit is reached.
#
# this is puzzling and may be caused by spurious page faults
# this function is a workaround and tries to find gadget page with no distinct
# auth_password -> mm_answer_authpassword backjump.
#
def _find_no_direct_backjump(run_list, run_set,
                            gt_auth_password,
                            gt_mm_answer_authpassword,
                            debug=False):
    auth_password_list = []
    mm_answer_authpassword_list = []

    for addr in run_set:
        if 6 <= run_list.count(addr) <= 9:
            mm_answer_authpassword_list.append(addr)
        if 30 <= run_list.count(addr) <= 30:
            auth_password_list.append(addr)

    if debug:
        print(f"no direct: auth_password_list: {len(auth_password_list)}")
        print(f"no direct: mm_answer_authpassword_list: {len(mm_answer_authpassword_list)}")

    candidates = []
    for auth_password in auth_password_list:
        for mm_answer_authpassword in mm_answer_authpassword_list:
            num_found = 0

            if auth_password == mm_answer_authpassword:
                continue

            for i in range(len(run_list) - 1):
                # mm_anser_authpasswd -> auth_pwd .... mm_anser_authpasswd
                if run_list[i] == mm_answer_authpassword and \
                        run_list[i + 1] == auth_password:
                    # first auth_password in list
                    if run_list[:i + 1].count(auth_password) == 0:
                        j = i + 1
                        while j < len(run_list):
                            if run_list[j] == mm_answer_authpassword:
                                num_found += 1
                                break
                            j = j + 1

            # combination must be unique
            if num_found == 1:
                candidates.append((mm_answer_authpassword, auth_password))

    return candidates


#
# find direct backjump auth_password -> mm_answer_authpassword
#
def _find_direct_backjump(run_list,
                         run_set,
                         gt_auth_password,
                         gt_mm_answer_authpassword,
                         debug=False):
    auth_password_list = []
    mm_answer_authpassword_list = []

    for addr in run_set:
        if 5 <= run_list.count(addr) <= 11:
            mm_answer_authpassword_list.append(addr)
        if 5 <= run_list.count(addr) <= 7:
            auth_password_list.append(addr)

    if debug:
        print(f"direct: auth_password_list: {len(auth_password_list)}")
        print(f"direct: mm_answer_authpassword_list: {len(mm_answer_authpassword_list)}")

    candidates = []
    for auth_password in auth_password_list:
        for mm_answer_authpassword in mm_answer_authpassword_list:
            num_found = 0
            found_candidate = True

            if auth_password == mm_answer_authpassword:
                continue

            for i in range(len(run_list) - 1):
                # back_jump: auth_pw -> mm_answer_authpw
                if run_list[i] == auth_password and run_list[i + 1] == mm_answer_authpassword:
                    num_found += 1

                # forward jump mm_answer_authpassword -> auth_password
                if run_list[i] == mm_answer_authpassword and run_list[i + 1] == auth_password:
                    if num_found > 0:
                        found_candidate = False
                        break
                    num_found += 1

            if num_found == 2 and found_candidate:
                candidates.append((mm_answer_authpassword, auth_password))
                break

    return candidates


def show_hint(candidates, run_list, gt_auth_password, gt_mm_answer_authpassword, debug=False):
    if debug:
        print(f"gt in list: auth_password: {run_list.count(gt_auth_password)}")
        print(f"gt in list: mm_answer_authpassword: {run_list.count(gt_mm_answer_authpassword)}")

    i = 1
    for c in candidates:
        mm_answer_authpassword = c[0]
        auth_password = c[1]

        gt_hint = ""
        if gt_auth_password == auth_password and \
                gt_mm_answer_authpassword == mm_answer_authpassword:
            gt_hint = " <-- this"

        print(f"[{i}] \tcandidate: mm_answer_authpassword = {hex(mm_answer_authpassword)} "
              f"-> auth_password = {hex(auth_password)} {gt_hint}")
        i = i + 1


if __name__ == '__main__':
    suffix = '2024-04-15T21-03-17-163705'
    file = f'/home/andrin/proj/heckler-new/sev-step/userspace/attack/heckler/data/trace_list_{suffix}'

    sx = OrderedSet()
    lx = load_json(f'{suffix}.json')
    for l in lx:
        sx.add(l)
    truth = load_json(file)
    c = find_candidates(lx, sx, truth['auth_password'], truth['mm_answer_authpassword'])
