import random
import subprocess
import timeit

import matplotlib.pyplot as plt


def get_text_patterns(text_file, pattern_sizes):
    current_pattern_size_index = 0
    patterns = []
    with open(text_file, 'r', encoding='latin1') as text:
        for line in text:
            pattern_size = pattern_sizes[current_pattern_size_index]
            size_difference = len(line) - pattern_size
            if size_difference > 0:
                start_index = random.randrange(size_difference)
                pattern = line[start_index: start_index + pattern_size]
                if '\n' in pattern or '\\' in pattern or '%' in pattern or '`' in pattern or '"' in pattern or "'" in pattern or '[' in pattern or ']' in pattern:
                    continue
                patterns.append(pattern)
                current_pattern_size_index += 1
            if current_pattern_size_index >= len(pattern_sizes):
                break
    return patterns


def run(algorithm, pattern, text_file, edit=-1):
    if algorithm == "grep":
        command = 'grep "%s" "%s" -o | wc -l' % (pattern, text_file)
    else:
        command = './ptm -a %s %s -c "%s" "%s"' % (algorithm, '' if edit == -1 else '-e %d' % edit, pattern, text_file)
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    process.wait()
    x = process.stdout.read()
    print(x)
    return x


def get_run_time(runs, algorithm, pattern, text_file, edit=-1):
    timer = timeit.Timer(lambda: run(algorithm, pattern, text_file, edit))
    time = timer.timeit(number=runs) / runs
    return time


def main():
    text_file = '/home/pedro00dk/Downloads/tests/english.txt'
    patterns = [
        'by', 'this', 'compose', 'footnote', 'appendices', 'word-processing', 'Provisional Government',
        'The Committee for Salvation', 'But the reader must realize that',
        'What kind of a governmental structure', 'For the first few months of the new regime',
        'On the other hand, the masses of the people wanted',
        'Labour Group in the Fourth Imperial Duma, which was composed',
        'believe that society must progress by natural evolution toward',
        'Stormy was the all-night meeting of the Petrograd Soviet the 30th of',
        "I'm going to ask you to excuse me, and go and lie down for a little.  I",
        'of the intellectuals Lenin and Trotzky stood for insurrection. Even   urrection. Even   |',
        'cultivation of literature. Mencius tells us how the princes had made away with many of the records'
    ]

    text_file = '/home/pedro00dk/Downloads/tests/sources.txt'
    patterns = [
        'if', 'void', 'include', 'asm/irq.h', 'static u16', 'unsigned long', 'SERIAL_21285_NAME',
        'struct uart_port *port', 'static irqreturn_t serial21285',
        'while (!(status & 0x10) && max_count--)', 'struct circ_buf *xmit = &port->info->xmit;',
        'serial21285_get_options(struct uart_port *port, int',
        'serial21285_console_setup(struct console *co, char *options)',
        'static int exports_open(struct inode *inode, struct file *file)',
        'static ssize_t write_unexport(struct file *file, char *buf, size_t size)',
        'WORD0_SUPPORTED_ATTRS   | FATTR4_WORD0_TYPE         | FATTR4_WORD0_FH_EXPIRE_TYPE',
        'fh_compose(struct svc_fh *fhp, struct svc_export *exp, struct dentry *dentry, struct svc_fh',
        'int direct2indirect (struct reiserfs_transaction_handle *, struct inode *, struct path *, struct'
    ]

    text_file = '/home/pedro00dk/Downloads/tests/proteins.txt'
    patterns = [
        'PQ', 'MAAL', 'PPPRPD', 'HDPAGALQ', 'HDPAGALQAA', 'EEEEEEEEATDSE', 'SLTTVPQTQGQTTAQKV',
        'FCDVTIVADSLKFRGHKVIL', 'DDKMLRFVDKNGLVPSSSGTVYDRTTVLIE',
        'MNQLRAEERFCDVTIVADSLKFRGHKVILAACSPFLRDQF', 'GVERSPSAANPTLMTSLRTWPLPWALPSPTCLGPPIPPRRQRIFS',
        'CRQIQEEEDEKQRLQNEVRQLTEKLARVNENLARKIASRNEFDRTIAETE',
        'SISADDDLQESSRRPQRKSLYESDRLRELGPDGEEAEGPGAGDGPPRSFDWGYEERSGAH',
        'DEEVQERLDHLLCWLLEHRGRLEGGPGWLAEAIVTWRGHLTKLLTTPYERQEGWQLAASRFQGTL',
        'EDEDDDGQCGEHLPFLVGGEEGFHLIDHEAMSQGYVQHIISPDQIHLTINPGSTPMPRNIEGATLTLQSE',
        'GPVPTFQPFQRSISADDDLQESSRRPQRKSLYESFVSSSDRLRELGPDGEEAEGPGAGDGPPRSFDWGYEERSGAHSSAS',
        'EEVQERLDHLLCWLLEHRGRLEGGPGWLAEAIVTWRGHLTKLLTTPYERQEGWQLAASRFQGTLYLSEVETPNARAQRLARPPLLRELMY',
        'QAGLADGRTHTVLLRLRGPSRPSPALHLYVDCKLGDQHAGLPALAPIPPAEVDGLEIRTGQKAYLRMQGFVESMKIILGGSMARVGALSECPFQGDESIH'
    ]

    print('patterns -> count: %d:' % len(patterns))
    for pattern in patterns:
        print('(%d): %s' % (len(pattern), pattern))

    print('compiling program')
    ptm_source = '../src/ptm.cpp'
    ptm = 'ptm'
    compile_command = '%s %s %s -o %s' % ('g++', '-std=c++11 -O2', ptm_source, ptm)
    subprocess.call(compile_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

    exact_algorithms = ['grep', 'bf', 'kmp', 'bm', 'ac', 'so']
    print('algorithms:')
    print(exact_algorithms)

    run('grep', patterns[0], text_file)  # Just to load file

    runs = 4
    run_times = {}

    print('testing algorithms in %d runs' % runs)
    for algorithm in exact_algorithms:
        run_times[algorithm] = []
        for pattern in patterns:
            time = get_run_time(runs, algorithm, pattern, text_file)
            print('algorithm %s, pattern length %d, mean run time %f' % (algorithm, len(pattern), time))
            run_times[algorithm].append(time)
        print()

    x = [len(pattern) for pattern in patterns]
    for algorithm, results in run_times.items():
        plt.plot(x, results, label=algorithm, marker='x')
    plt.xlabel('pattern size')
    plt.ylabel('time (sec)')
    plt.legend()
    plt.grid()
    plt.show()

    """
    text_file = 'text_approx.txt'
    text_size = 5
    run_command = './%s %s %d %d' % (gen_text, text_file, text_size, text_sigma)
    subprocess.call(run_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    print('text file -> name: %s, size: %d mb, ab size: %d' % (text_file, text_size, text_sigma))

    patterns = get_text_patterns(text_file, [*range(5, 101, 5)])
    print('patterns -> count: %d:' % len(patterns))
    for pattern in patterns:
        print('(%d): %s' % (len(pattern), pattern))

    approx_algorithms = ['se', 'uk']
    print('algorithms:')
    print(approx_algorithms)

    run('grep', patterns[0], text_file)  # Just to load file

    print('testing algorithms in %d runs' % runs)
    for algorithm in approx_algorithms:
        run_times[algorithm] = []
        for pattern in patterns:
            time = get_run_time(runs, algorithm, pattern, text_file, 0)
            print('algorithm %s, pattern length %d, mean run time %f' % (algorithm, len(pattern), time))
            run_times[algorithm].append(time)
        print()

    x = [len(pattern) for pattern in patterns]
    for algorithm, results in run_times.items():
        plt.plot(x, results, label=algorithm)
    plt.xlabel('pattern size')
    plt.ylabel('time (sec)')
    plt.legend()
    plt.show()
    """


if __name__ == "__main__":
    main()
