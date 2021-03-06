#ifndef EXTERNALNODE_H
#define EXTERNALNODE_H

#include <QXmlStreamReader>

#include "node.h"
#include "metareader.h"

/**
 * @brief A node generated from an external XML metadata file
 */
class ExternalNode : public Node
{
public:
  ExternalNode(const QString& xml_meta_filename);

  virtual Node* copy() const override;

  virtual QString Name() const override;
  virtual QString id() const override;
  virtual QString Category() const override;
  virtual QString Description() const override;

  virtual void Retranslate() override;

  virtual bool IsAccelerated() const override;
  virtual QString AcceleratedCodeVertex() const override;
  virtual QString AcceleratedCodeFragment() const override;
  virtual int AcceleratedCodeIterations() const override;
  virtual NodeInput* AcceleratedCodeIterativeInput() const override;

private:
  NodeMetaReader meta_;
};

#endif // EXTERNALNODE_H
