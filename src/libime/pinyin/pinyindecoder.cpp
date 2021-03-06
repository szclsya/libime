/*
 * SPDX-FileCopyrightText: 2017-2017 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "pinyindecoder_p.h"
#include <cmath>

namespace libime {

PinyinLatticeNode::PinyinLatticeNode(
    std::string_view word, WordIndex idx, SegmentGraphPath path,
    const State &state, float cost,
    std::unique_ptr<PinyinLatticeNodePrivate> data)
    : LatticeNode(word, idx, std::move(path), state, cost),
      d_ptr(std::move(data)) {}

PinyinLatticeNode::~PinyinLatticeNode() = default;

const std::string &PinyinLatticeNode::encodedPinyin() const {
    static const std::string empty;
    if (!d_ptr) {
        return empty;
    }
    return d_ptr->encodedPinyin_;
}

LatticeNode *PinyinDecoder::createLatticeNodeImpl(
    const SegmentGraphBase &graph, const LanguageModelBase *model,
    std::string_view word, WordIndex idx, SegmentGraphPath path,
    const State &state, float cost, std::unique_ptr<LatticeNodeData> data,
    bool onlyPath) const {
    std::unique_ptr<PinyinLatticeNodePrivate> pinyinData(
        static_cast<PinyinLatticeNodePrivate *>(data.release()));
    if (model->isUnknown(idx, word)) {
        // we don't really care about a lot of unknown single character
        // which is not used for candidates
        if ((pinyinData && pinyinData->encodedPinyin_.size() == 2) &&
            path.front() != &graph.start() && !onlyPath) {
            return nullptr;
        }
    }

    return new PinyinLatticeNode(word, idx, std::move(path), state, cost,
                                 std::move(pinyinData));
}
} // namespace libime
