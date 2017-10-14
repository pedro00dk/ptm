import random
import subprocess
import timeit
from operator import itemgetter

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


def run(algorithm, pattern, text_file, edit=-1, pattern_file=False):
    if algorithm == "grep":
        command = 'grep %s "%s" "%s" -o | wc -l' % ('-f' if pattern_file else '',
                                                    pattern,
                                                    text_file)
    else:
        command = './ptm -a %s %s -c %s "%s" "%s"' % (algorithm,
                                                      '' if edit == -1 else '-e %d' % edit,
                                                      '-p' if pattern_file else '',
                                                      pattern,
                                                      text_file)
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    process.wait()
    x = process.stdout.read()
    print(x)
    return x


def get_run_time(runs, algorithm, pattern, text_file, edit=-1, pattern_file=False):
    timer = timeit.Timer(lambda: run(algorithm, pattern, text_file, edit, pattern_file))
    time = timer.timeit(number=runs) / runs
    return time


def choose_sample(sample):
    text_file = {}
    patterns = {}

    text_file['english'] = 'samples/english.txt'
    patterns['english'] = [
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

    text_file['sources'] = 'samples/sources.txt'
    patterns['sources'] = [
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
    text_file['proteins'] = 'samples/proteins.100MB'
    patterns['proteins'] = [
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
    return text_file[sample], patterns[sample]


def run_and_plot_exact(text_file, patterns, exact_algorithms, runs):
    run_times = {}

    run('grep', patterns[0], text_file)  # Just to load file

    print('testing algorithms in %d runs' % runs)
    for algorithm in exact_algorithms:
        run_times[algorithm] = []
        for pattern in patterns:
            time = get_run_time(runs, algorithm, pattern, text_file)
            print('algorithm %s, pattern length %d, mean run time %f' % (algorithm, len(pattern), time))
            run_times[algorithm].append(time)
        print()

    x = [len(pattern) for pattern in patterns]
    for algorithm, results in sorted(run_times.items()):
        plt.plot(x, results, label=algorithm, marker='x')
    plt.xlabel('pattern size')
    plt.ylabel('time (sec)')
    plt.legend()
    plt.grid()
    # plt.show()
    plt.savefig("results_exact.png")
    plt.clf()


def run_and_plot_exact_pattern_file(text_file, patterns, exact_algorithms, runs):
    run_times = {}

    patterns = patterns[:2]

    patterns_filename = 'patterns.txt'
    with open(patterns_filename, 'w') as patterns_file:
        for pattern in patterns:
            patterns_file.write(pattern)
            patterns_file.write('\n')

    run('grep', patterns[0], text_file)  # Just to load file

    print('testing algorithms in %d runs' % runs)
    for algorithm in exact_algorithms:
        run_times[algorithm] = []
        time = get_run_time(runs, algorithm, patterns_filename, text_file, pattern_file=True)
        print('algorithm %s, pattern file, mean run time %f' % (algorithm, time))
        run_times[algorithm].append(time)
        print()

    print(run_times)
    x = list(range(len(exact_algorithms)))
    x_names = []
    y = []

    for (algorithm, result) in sorted(run_times.items(), key=itemgetter(1), reverse=True):
        x_names.append(algorithm)
        y.append(result)

    plt.xticks(x, x_names)
    plt.xlabel('algorithm')
    plt.ylabel('time (sec)')
    plt.legend()
    plt.grid()
    plt.plot(x, y, 'g', marker='x')
    # plt.show()
    # TODO: plotar barchart
    plt.savefig("results_exact_pattern_file.png")
    plt.clf()


def run_and_plot_aprox(text_file, patterns, approx_algorithms, runs, max_edit_distance):
    run_times = {}

    run('grep', patterns[0], text_file)  # Just to load file

    print('testing algorithms in %d runs' % runs)
    for algorithm in approx_algorithms:
        run_times[algorithm] = []
        for pattern in patterns:
            time = get_run_time(runs, algorithm, pattern, text_file, max_edit_distance)
            print('algorithm %s, pattern length %d, mean run time %f' % (algorithm, len(pattern), time))
            run_times[algorithm].append(time)
        print()

    x = [len(pattern) for pattern in patterns]
    for algorithm, results in sorted(run_times.items()):
        plt.plot(x, results, label=algorithm, marker='x')
    plt.xlabel('pattern size')
    plt.ylabel('time (sec)')
    plt.legend()
    plt.grid()
    # plt.show()
    plt.savefig('results_aprox' + str(max_edit_distance) + '.png')
    plt.clf()


def main():
    text_file, patterns = choose_sample('proteins')

    print('patterns -> count: %d:' % len(patterns))
    for pattern in patterns:
        print('(%d): %s' % (len(pattern), pattern))

    print('compiling program')
    ptm_source = '../src/ptm.cpp'
    ptm = 'ptm'
    compile_command = '%s %s %s -o %s' % ('g++', '-std=c++11 -O2', ptm_source, ptm)
    subprocess.call(compile_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

    exact_algorithms = ['grep', 'bf', 'kmp', 'bm', 'ac', 'so', 'rk']
    print('algorithms:')
    print(exact_algorithms)

    runs = 5
    run_and_plot_exact(text_file, patterns, exact_algorithms, runs)
    run_and_plot_exact_pattern_file(text_file, patterns, exact_algorithms, runs)

    approx_algorithms = ['se', 'uk']
    print('algorithms:')
    print(approx_algorithms)

    patterns = patterns[:8]
    runs = 5
    max_edit_distance = 2
    for edit_distance in range(max_edit_distance + 1):
        run_and_plot_aprox(text_file, patterns, approx_algorithms, runs, edit_distance)


if __name__ == "__main__":
    main()
