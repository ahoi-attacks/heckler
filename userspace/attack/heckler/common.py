import json
import os
from datetime import datetime

import ordered_set

script_dir = os.path.dirname(os.path.realpath(__file__))
time_prefix = datetime.now().isoformat().replace(':', '-').replace('.', '-')

DEBUG = True
TRACE = True


def debug(*vargs):
    if (DEBUG):
        print(vargs)


def load_json(path):
    with open(path, "r") as f:
        txt = f.read()
        lx = json.loads(txt)
        return lx


class SetEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, set):
            return list(obj)
        if isinstance(obj, ordered_set.OrderedSet):
            return list(obj)
        return json.JSONEncoder.default(self, obj)


def load_entries(path):
    with open(path, "r") as f:
        txt = f.read()
        l = json.loads(txt)
        return ordered_set.OrderedSet(l)


def dump_entries(name, data, time_suffix=True, subfolder=''):
    dump_json = json.dumps(data, cls=SetEncoder)
    print('dumping entries ', name, ' ', len(data))

    folder_name = script_dir + '/data/' + subfolder
    os.makedirs(folder_name, exist_ok=True)

    filename = folder_name + name
    if time_suffix:
        filename = filename + '_' + time_prefix + '.json'
    else:
        filename = filename + '.json'

    with open(filename, "w") as f:
        print(filename)
        f.write(dump_json)

    return filename


def dump_sshd_target_page(name,
                          mm_answer_authpassword,
                          page_sshd_auth_password,
                          add_date, more_context={}):
    d = {
        'page_sshd_mm_answer_authpassword': mm_answer_authpassword,
        'page_sshd_auth_password': page_sshd_auth_password,
        'more': more_context
    }
    dump_entries(name, d, add_date)


def restore_sshd_target_page(path):
    with open(path, "r") as f:
        txt = f.read()
        lx = json.loads(txt)
        return lx
