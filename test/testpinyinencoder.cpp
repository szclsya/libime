/*
 * SPDX-FileCopyrightText: 2017-2017 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "libime/core/lattice.h"
#include "libime/pinyin/pinyinencoder.h"
#include <fcitx-utils/log.h>

using namespace libime;

void dfs(const SegmentGraph &segs) {
    FCITX_ASSERT(segs.checkGraph());

    auto callback = [](const SegmentGraphBase &segs,
                       const std::vector<size_t> &path) {
        size_t s = 0;
        for (auto e : path) {
            std::cout << segs.segment(s, e) << " ";
            s = e;
        }
        std::cout << std::endl;
        return true;
    };

    segs.dfs(callback);
}

void check(std::string py, PinyinFuzzyFlags flags) {
    dfs(PinyinEncoder::parseUserPinyin(std::move(py), flags));
}

int main() {
    check("wa'nan'''", PinyinFuzzyFlag::None);
    check("lvenu", PinyinFuzzyFlag::None);
    check("woaizuguotiananmen", PinyinFuzzyFlag::None);
    check("wanan", PinyinFuzzyFlag::None);
    check("biiiiiilp", PinyinFuzzyFlag::None);
    check("zhm", PinyinFuzzyFlag::None);
    check("zzhzhhzhzh", PinyinFuzzyFlag::None);
    check("shuou", PinyinFuzzyFlag::None);
    check("tanan", PinyinFuzzyFlag::None);
    check("lven", PinyinFuzzyFlag::None);
    check("ananananana", PinyinFuzzyFlag::None);
    check("wa'nan", PinyinFuzzyFlag::None);
    check("xian", PinyinFuzzyFlag::None);
    check("xian", PinyinFuzzyFlag::Inner);
    check("xi'an", PinyinFuzzyFlag::Inner);
    check("kuai", PinyinFuzzyFlag::None);
    check("kuai", PinyinFuzzyFlag::Inner);
    check("jiaou", PinyinFuzzyFlag::Inner);
    check("jin'an", PinyinFuzzyFlag::Inner);

    for (auto syl : PinyinEncoder::stringToSyllables(
             "niagn",
             PinyinFuzzyFlags{PinyinFuzzyFlag::L_N, PinyinFuzzyFlag::IAN_IANG,
                              PinyinFuzzyFlag::NG_GN})) {
        for (auto f : syl.second) {
            std::cout << PinyinSyllable(syl.first, f.first).toString()
                      << std::endl;
        }
    }
    for (auto syl : PinyinEncoder::stringToSyllables(
             "n",
             PinyinFuzzyFlags{PinyinFuzzyFlag::L_N, PinyinFuzzyFlag::IAN_IANG,
                              PinyinFuzzyFlag::NG_GN})) {
        for (auto f : syl.second) {
            std::cout << PinyinSyllable(syl.first, f.first).toString()
                      << std::endl;
        }
    }
    for (auto syl : PinyinEncoder::stringToSyllables(
             "cuagn", {PinyinFuzzyFlag::C_CH, PinyinFuzzyFlag::UAN_UANG,
                       PinyinFuzzyFlag::NG_GN})) {
        for (auto f : syl.second) {
            std::cout << PinyinSyllable(syl.first, f.first).toString()
                      << std::endl;
        }
    }
    {
        // xiang o n
        check("xian", PinyinFuzzyFlag::None);

        // xian gong
        check("xiangong", PinyinFuzzyFlag::None);

        // xiang o n
        check("xiangon", PinyinFuzzyFlag::None);

        // yan d
        check("yand", PinyinFuzzyFlag::None);
        // hua c o
        check("huaco", PinyinFuzzyFlag::None);
        // hua c o
        check("xion", PinyinFuzzyFlag::None);
        check("xiana", PinyinFuzzyFlag::None);
    }

    {
        auto graph = PinyinEncoder::parseUserPinyin("", PinyinFuzzyFlag::None);
        {
            auto graph2 =
                PinyinEncoder::parseUserPinyin("z", PinyinFuzzyFlag::None);
            graph.merge(graph2);
        }
        dfs(graph);
        {
            auto graph2 =
                PinyinEncoder::parseUserPinyin("zn", PinyinFuzzyFlag::None);
            graph.merge(graph2);
        }
        dfs(graph);
        {
            auto graph2 =
                PinyinEncoder::parseUserPinyin("z", PinyinFuzzyFlag::None);
            graph.merge(graph2);
        }
        dfs(graph);
    }
    {
        auto result =
            PinyinEncoder::stringToSyllables("z", PinyinFuzzyFlag::None);
        for (auto p : result) {
            for (auto f : p.second) {
                std::cout << PinyinEncoder::initialToString(p.first)
                          << PinyinEncoder::finalToString(f.first) << std::endl;
            }
        }
    }
    {
        auto result = PinyinEncoder::encodeOneUserPinyin("nihao");
        FCITX_ASSERT(PinyinEncoder::decodeFullPinyin(result) == "ni'hao");
    }
    {
        auto result = PinyinEncoder::encodeOneUserPinyin("xian");
        FCITX_ASSERT(PinyinEncoder::decodeFullPinyin(result) == "xian");
    }
    {
        auto result = PinyinEncoder::encodeOneUserPinyin("xi'an");
        FCITX_ASSERT(PinyinEncoder::decodeFullPinyin(result) == "xi'an");
    }
    {
        auto result = PinyinEncoder::encodeOneUserPinyin("nh");
        FCITX_ASSERT(PinyinEncoder::decodeFullPinyin(result) == "n'h");
    }
    {
        auto result = PinyinEncoder::encodeOneUserPinyin("nfi");
        FCITX_INFO() << PinyinEncoder::decodeFullPinyin(result);
    }
    return 0;
}
