import glob
import os

import pytest
import numpy as np
import heckler.common
from heckler.sudo.sudo_attack import SudoAttack
from heckler.ssh.trace_eval import _find_direct_backjump
from ordered_set import OrderedSet

script_dir = os.path.dirname(os.path.realpath(__file__))


def eval_ssh_data():
    src_files = script_dir + '/res/ssh/backjump/vm_profile_sshd_run_*.json'
    files = glob.glob(src_files)

    run_list_sizes = []
    page_sshd_mm_answer_authpassword = []
    page_sshd_auth_password = []
    xcrypt = []
    auth_password = []
    mm_answer_authpassword = []
    boot_set = []

    data = []

    i = 0
    for name in files:
        print(i)
        i = i + 1
        f = heckler.common.load_json(name)
        run_list = f['run_list']
        page_sshd_mm_answer_authpassword.append(run_list.count(f['sshd_pages']['page_sshd_mm_answer_authpassword']))
        page_sshd_auth_password.append(run_list.count(f['sshd_pages']['page_sshd_auth_password']))
        xcrypt.append(run_list.count(f['sshd_pages']['more']['xcrypt']))
        auth_password.append(run_list.count(f['sshd_pages']['more']['auth_password']))
        mm_answer_authpassword.append(run_list.count(f['sshd_pages']['more']['mm_answer_authpassword']))
        boot = f['boot_set']
        boot_set.append(len(boot))
        run_list_sizes.append(len(run_list))

        data.append({
            'run_list': run_list,
            'mm_answer_authpassword': f['sshd_pages']['page_sshd_mm_answer_authpassword'],
            'mm_answer_authpassword_count': mm_answer_authpassword[-1],
            'auth_password_count': auth_password[-1],
            'auth_password': f['sshd_pages']['page_sshd_auth_password']
        })

    print('number of trace files', len(files))
    tot = np.mean(run_list_sizes)

    print('run list', tot)
    print('page_sshd_mm_answer_authpassword', np.mean(page_sshd_mm_answer_authpassword),
          np.mean(page_sshd_mm_answer_authpassword) / tot * 100)
    print('page_sshd_auth_password', np.mean(page_sshd_auth_password), np.mean(page_sshd_auth_password) / tot * 100)
    print('xcrypt', np.mean(xcrypt), np.mean(xcrypt) / tot * 100)
    print('auth_password', np.mean(auth_password), np.mean(auth_password) / tot * 100)
    print('mm_answer_authpassword', np.mean(mm_answer_authpassword), np.mean(mm_answer_authpassword) / tot * 100)
    print('boot set size: ', np.mean(boot_set))

    return data


ssh_data_direct_jump = eval_ssh_data()


@pytest.mark.parametrize('data', ssh_data_direct_jump)
def test_direct_backjump(data):
    run_list = data['run_list']
    auth_password = data['auth_password']
    mm_answer_authpassword = data['mm_answer_authpassword']

    print(f'using: auth_password={hex(auth_password)}')
    print(f'using: mm_answer_authpassword={hex(mm_answer_authpassword)}')

    run_set = OrderedSet()
    for l in run_list:
        run_set.add(l)

    candidates = _find_direct_backjump(run_list=run_list, run_set=run_set,
                                       gt_auth_password=auth_password,
                                       gt_mm_answer_authpassword=mm_answer_authpassword,
                                       debug=True)

    assert (mm_answer_authpassword, auth_password) in candidates

    pass


def load_data_no_direct_backjump():
    pass


ssh_data_nodirect_jump = load_data_no_direct_backjump()


def test_nodirect_backjump():
    pass
