#ifndef CAFFE_DETECTION_OUTPUT_LAYER_HPP_
#define CAFFE_DETECTION_OUTPUT_LAYER_HPP_

#include <string>
#include <vector>

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"

namespace caffe {

/**
 * @brief Generate the detection output based on location and confidence
 * predictions by doing non maximum suppression.
 *
 * Intended for use with MultiBox detection method.
 *
 * NOTE: does not implement Backwards operation.
 */
template <typename Dtype>
class DetectionOutputLayer : public Layer<Dtype> {
 public:
  explicit DetectionOutputLayer(const LayerParameter& param)
      : Layer<Dtype>(param) {}
  virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);

  virtual inline const char* type() const { return "DetectionOutput"; }
  virtual inline int ExactBottomBlobs() const { return 3; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
  /**
   * @brief Do non maximum suppression (nms) on prediction results.
   *
   * @param bottom input Blob vector (at least 2)
   *   -# @f$ (N \times C1 \times 1 \times 1) @f$
   *      the location predictions with C1 predictions.
   *   -# @f$ (N \times C2 \times 1 \times 1) @f$
   *      the confidence predictions with C2 predictions.
   *   -# @f$ (N \times 2 \times C3 \times 1) @f$
   *      the prior bounding boxes with C3 values.
   * @param top output Blob vector (length 1)
   *   -# @f$ (1 \times 1 \times N \times 7) @f$
   *      N is the number of detections after nms, and each row is:
   *      [image_id, label, confidence, xmin, ymin, xmax, ymax]
   */
  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  /// @brief Not implemented
  virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
    NOT_IMPLEMENTED;
  }

  int num_classes_;
  bool share_location_;
  int loc_classes_;
  int background_label_id_;

  int num_;
  int num_priors_;

  float nms_threshold_;
  int top_k_;

  bool need_save_;
  string output_directory_;
  string output_name_prefix_;
  string output_format_;
  map<int, string> label_to_name_;
  vector<string> names_;
  vector<pair<int, int> > sizes_;
  int name_count_;
};

}  // namespace caffe

#endif  // CAFFE_DETECTION_OUTPUT_LAYER_HPP_